#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/tree.h"
#include "../../../inc/parsingAnalysis/ast/vector/ast_index.h"
#include <cstddef>
#include <memory>
#include <variant>

/**

FillSemanticInfo ---> insertar delcaraciones en las tablas / insertar tipos /
comprobar que las variables pertenecen al scope (llamadas a variables) /
comrpobar llamadas a enum

TypeAnalysis ---> comprobar en una llamada a funcion que esta existe debido a
sobrecarga de funciones requiere que se trate en el typeAnalysis / igual con
metodos / llamadas a atributos / variables auto

 */

namespace nicole {

std::expected<std::string, Error>
CodeGeneration::nameMangling(const std::shared_ptr<Type> &type) const noexcept {
  std::string mangled{};
  auto res = nameManglingImpl(type, mangled);
  if (!res)
    return res;
  // quitar posible guión bajo inicial
  if (!mangled.empty() && mangled.front() == '_')
    mangled.erase(mangled.begin());
  return mangled;
}

std::expected<std::string, Error>
CodeGeneration::nameManglingImpl(const std::shared_ptr<Type> &type,
                                 std::string &result) const noexcept {
  if (!type)
    return createError(ERROR_TYPE::TYPE, "null type in name mangling");

  if (auto bt = std::dynamic_pointer_cast<BasicType>(type)) {
    result += '_' + bt->toString();
    return result;
  }
  if (std::dynamic_pointer_cast<VoidType>(type)) {
    result += "_void";
    return result;
  }
  if (std::dynamic_pointer_cast<NullType>(type)) {
    result += "_null";
    return result;
  }
  if (auto pt = std::dynamic_pointer_cast<PointerType>(type)) {
    auto rec = nameManglingImpl(pt->baseType(), result);
    if (!rec)
      return rec;
    result += "_ptr";
    return result;
  }
  if (auto vt = std::dynamic_pointer_cast<VectorType>(type)) {
    auto rec = nameManglingImpl(vt->elementType(), result);
    if (!rec)
      return rec;
    result += "_vec";
    return result;
  }
  if (auto ct = std::dynamic_pointer_cast<ConstType>(type)) {
    result += "_const";
    auto rec = nameManglingImpl(ct->baseType(), result);
    if (!rec)
      return rec;
    return result;
  }
  if (auto et = std::dynamic_pointer_cast<EnumType>(type)) {
    result += '_' + et->name();
    return result;
  }
  if (auto git = std::dynamic_pointer_cast<GenericInstanceType>(type)) {
    result += '_' + git->name();
    for (auto &arg : git->typeArgs()) {
      auto rec = nameManglingImpl(arg, result);
      if (!rec)
        return rec;
    }
    return result;
  }
  if (auto ut = std::dynamic_pointer_cast<UserType>(type)) {
    result += '_' + ut->name();
    return result;
  }
  // Fallback
  std::string raw = type->toString();
  for (char c : raw) {
    result += (std::isalnum(static_cast<unsigned char>(c)) ? c : '_');
  }
  return result;
}

std::expected<std::string, Error>
CodeGeneration::nameManglingFunction(const Function &func,
                                     const std::vector<std::shared_ptr<Type>>
                                         &genericReplacements) const noexcept {
  std::string mangled{"$"};
  auto res = nameManglingFunctionImpl(func, genericReplacements, mangled);
  if (!res)
    return res;
  return mangled;
}

std::expected<std::string, Error> CodeGeneration::nameManglingFunctionImpl(
    const Function &func,
    const std::vector<std::shared_ptr<Type>> &genericReplacements,
    std::string &result) const noexcept {
  result += '_';
  result += func.id();

  for (const auto &genType : genericReplacements) {
    auto resType = nameMangling(genType);
    if (!resType)
      return createError(resType.error());
    result += '_';
    result += *resType;
  }

  for (const auto &param : func.params().params()) {
    auto resParam = nameMangling(param.second);
    if (!resParam)
      return createError(resParam.error());
    result += '_';
    result += *resParam;
  }

  auto resRet = nameMangling(func.returnType());
  if (!resRet)
    return createError(resRet.error());
  result += "_ret_";
  result += *resRet;

  return result;
}

bool isVectorElement(const AST *node) noexcept {
  return dynamic_cast<const AST_INDEX*>(node) != nullptr;
}

// EMIT LVALUE: simplemente delega al visitor y devuelve la dirección
std::expected<llvm::Value *, Error>
CodeGeneration::emitLValue(const AST *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST for lvalue");
  // asumimos que el visit de ese nodo produce *siempre* un
  // AllocaInst*/dirección
  return node->accept(*this);
}

// EMIT RVALUE: carga una vez la dirección devuelta por emitLValue
std::expected<llvm::Value *, Error>
CodeGeneration::emitRValue(const AST *node) const noexcept {
  auto valOrErr = emitLValue(node);
  if (!valOrErr) return createError(valOrErr.error());
  llvm::Value *val = *valOrErr;

  auto tyOrErr = node->returnedFromTypeAnalysis()->llvmVersion(context_);
  if (!tyOrErr) return createError(tyOrErr.error());
  llvm::Type *llvmTy = *tyOrErr;

  // Agregados completos…
  if (llvmTy->isAggregateType()) {
    llvm::AllocaInst *tmp =
      builder_.CreateAlloca(llvmTy, nullptr, "agg_tmp");
    const auto &DL = module_->getDataLayout();
    builder_.CreateMemCpy(tmp, llvm::MaybeAlign(),
                          val, llvm::MaybeAlign(),
                          DL.getTypeAllocSize(llvmTy));
    return tmp;
  }

  // Si es puntero:
  if (val->getType()->isPointerTy()) {
    // ¡Pero sólo cargamos si NO es una constante!
    if (!llvm::isa<llvm::Constant>(val)  && !isVectorElement(node)) {
      // esto cubre variables locales (alloca), argumentos, etc.
      return builder_.CreateLoad(llvmTy, val, "rval_load");
    }
    // en cambio, literales de cadena (CreateGlobalStringPtr → Constant*)
    // se dejan tal cual, como i8* apuntando al .rodata
    return val;
  }

  // escalar ya cargado (int, float, bool…)
  return val;
}


std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT");
  }
  return node->expression()->accept(*this);
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BODY");
  }
  currentScope_ = node->scope();

  llvm::Value *lastValue{nullptr};

  // Recorremos todas las sentencias del cuerpo
  for (const auto &statement : node->body()) {
    auto res = statement->accept(*this);
    if (!res) {
      // Propagamos el error si alguna sentencia falla
      return createError(res.error());
    }
    if (builder_.GetInsertBlock()->getTerminator())
      break;
    // Guardamos el valor devuelto (puede ser nullptr si la sentencia no produce
    // valor)
    lastValue = res.value();
  }

  // Devolvemos el valor de la última sentencia (o nullptr si no había
  // sentencias)
  return lastValue;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }

  funcType_ = llvm::FunctionType::get(
      (options_.validateTree()) ? builder_.getInt32Ty() : builder_.getVoidTy(),
      false);
  mainFunction_ = llvm::Function::Create(
      funcType_, llvm::Function::ExternalLinkage, "main", module_.get());
  entry_ = llvm::BasicBlock::Create(context_, "entry", mainFunction_);
  builder_.SetInsertPoint(entry_);

  const auto result{tree->root()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }

  if (!options_.validateTree()) {
    builder_.CreateRetVoid();
  }

  llvm::verifyModule(*module_, &llvm::errs());

  module_->print(llvm::outs(), nullptr);

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  std::string errStr;
  llvm::ExecutionEngine *execEngine =
      llvm::EngineBuilder(std::move(module_))
          .setErrorStr(&errStr)
          .setOptLevel(llvm::CodeGenOptLevel::Default)
          .create();
  if (!execEngine) {
    std::cerr << "Failed to create ExecutionEngine: " << errStr << std::endl;
    return nullptr;
  }

  // Ejecuta la función main y obtiene el resultado
  std::vector<llvm::GenericValue> noargs;
  llvm::GenericValue gv = execEngine->runFunction(mainFunction_, noargs);

  delete execEngine;

  return nullptr;
}

} // namespace nicole