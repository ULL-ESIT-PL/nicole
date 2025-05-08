#include "../../../inc/parsingAnalysis/ast/operators/ast_binary.h"
#include "../../../inc/parsingAnalysis/ast/operators/ast_unary.h"
#include "../../../inc/visitors/codeGeneration/codeGeneration.h"
#include <memory>

namespace nicole {

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_BINARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BINARY");
  }

  // Generar subexpresiones
  auto leftOrErr = node->left()->accept(*this);
  if (!leftOrErr)
    return createError(leftOrErr.error());
  llvm::Value *L = *leftOrErr;

  auto rightOrErr = node->right()->accept(*this);
  if (!rightOrErr)
    return createError(rightOrErr.error());
  llvm::Value *R = *rightOrErr;

  // Obtener tipo semántico resultante (desenrollando ConstType)
  auto semTy = node->returnedFromTypeAnalysis();
  if (auto c = std::dynamic_pointer_cast<ConstType>(semTy))
    semTy = c->baseType();

  // Si es BasicType, usar operaciones LLVM correspondientes
  if (auto basic = std::dynamic_pointer_cast<BasicType>(semTy)) {
    switch (basic->baseKind()) {
    case BasicKind::Int: {
      // Enteros: + - * / % & | comparaciones
      switch (node->op().type()) {
      case TokenType::OPERATOR_ADD:
        return builder_.CreateAdd(L, R, "addtmp");
      case TokenType::OPERATOR_SUB:
        return builder_.CreateSub(L, R, "subtmp");
      case TokenType::OPERATOR_MULT:
        return builder_.CreateMul(L, R, "multmp");
      case TokenType::OPERATOR_DIV:
        return builder_.CreateSDiv(L, R, "divtmp");
      case TokenType::OPERATOR_MODULE:
        return builder_.CreateSRem(L, R, "modtmp");
      case TokenType::AND:
        return builder_.CreateAnd(L, R, "andtmp");
      case TokenType::OR:
        return builder_.CreateOr(L, R, "ortmp");
      case TokenType::EQUAL:
        return builder_.CreateICmpEQ(L, R, "eqtmp");
      case TokenType::NOTEQUAL:
        return builder_.CreateICmpNE(L, R, "netmp");
      case TokenType::OPERATOR_SMALLER:
        return builder_.CreateICmpSLT(L, R, "sltmp");
      case TokenType::SMALLEREQUAL:
        return builder_.CreateICmpSLE(L, R, "sletmp");
      case TokenType::OPERATOR_GREATER:
        return builder_.CreateICmpSGT(L, R, "sgtmp");
      case TokenType::BIGGEREQUAL:
        return builder_.CreateICmpSGE(L, R, "sgetmp");
      default:
        break;
      }
      break;
    }
    case BasicKind::Float:
    case BasicKind::Double: {
      // Flotantes: + - * / comparaciones
      switch (node->op().type()) {
      case TokenType::OPERATOR_ADD:
        return builder_.CreateFAdd(L, R, "faddtmp");
      case TokenType::OPERATOR_SUB:
        return builder_.CreateFSub(L, R, "fsubtmp");
      case TokenType::OPERATOR_MULT:
        return builder_.CreateFMul(L, R, "fmultmp");
      case TokenType::OPERATOR_DIV:
        return builder_.CreateFDiv(L, R, "fdivtmp");
      case TokenType::EQUAL:
        return builder_.CreateFCmpUEQ(L, R, "feqtmp");
      case TokenType::NOTEQUAL:
        return builder_.CreateFCmpUNE(L, R, "fnetmp");
      case TokenType::OPERATOR_SMALLER:
        return builder_.CreateFCmpULT(L, R, "flttmp");
      case TokenType::SMALLEREQUAL:
        return builder_.CreateFCmpULE(L, R, "fletmp");
      case TokenType::OPERATOR_GREATER:
        return builder_.CreateFCmpUGT(L, R, "fgtmp");
      case TokenType::BIGGEREQUAL:
        return builder_.CreateFCmpUGE(L, R, "fgetmp");
      default:
        break;
      }
      break;
    }
    case BasicKind::Bool: {
      // Booleanos: && || == !=
      switch (node->op().type()) {
      case TokenType::AND:
        return builder_.CreateAnd(L, R, "andtmp");
      case TokenType::OR:
        return builder_.CreateOr(L, R, "ortmp");
      case TokenType::EQUAL:
        return builder_.CreateICmpEQ(L, R, "eqtmp");
      case TokenType::NOTEQUAL:
        return builder_.CreateICmpNE(L, R, "netmp");
      default:
        break;
      }
      break;
    }
    case BasicKind::Str: {
      if (node->op().type() == TokenType::OPERATOR_ADD) {
        // Contexto y tipos LLVM
        llvm::LLVMContext &C = context_;
        llvm::Module &M = *module_;
        llvm::Type *i8Ty = llvm::Type::getInt8Ty(C);
        llvm::PointerType *i8Ptr = i8Ty->getPointerTo(); // i8*
        llvm::Type *i64Ty = llvm::Type::getInt64Ty(C);

        // Declarar strlen y malloc si no existen
        auto *strlenFT = llvm::FunctionType::get(i64Ty, {i8Ptr}, false);
        auto *mallocFT = llvm::FunctionType::get(i8Ptr, {i64Ty}, false);
        auto strlenFn = M.getOrInsertFunction("strlen", strlenFT);
        auto mallocFn = M.getOrInsertFunction("malloc", mallocFT);

        // Calcular longitudes de L y R
        llvm::Value *lenL = builder_.CreateCall(strlenFn, {L}, "lenL");
        llvm::Value *lenR = builder_.CreateCall(strlenFn, {R}, "lenR");

        // Tamaño = lenL + lenR + 1
        llvm::Value *sumLR = builder_.CreateAdd(lenL, lenR, "sumLR");
        llvm::Value *size = builder_.CreateAdd(
            sumLR, llvm::ConstantInt::get(i64Ty, 1), "allocSize");

        // Llamar a malloc
        llvm::CallInst *buf = builder_.CreateCall(mallocFn, {size}, "buf");

        // Copiar L en buf
        builder_.CreateMemCpy(
            /*Dest=*/buf, llvm::MaybeAlign(),
            /*Src=*/L, llvm::MaybeAlign(),
            /*Size=*/lenL);

        // Copiar R en buf + lenL
        llvm::Value *dstR = builder_.CreateGEP(
            /*PointeeType=*/i8Ty,
            /*Ptr=*/buf,
            /*Index=*/lenL, "dstR");
        builder_.CreateMemCpy(
            /*Dest=*/dstR, llvm::MaybeAlign(),
            /*Src=*/R, llvm::MaybeAlign(),
            /*Size=*/lenR);

        // Escribir '\0' al final: buf[lenL+lenR]
        llvm::Value *sumLR2 = builder_.CreateAdd(lenL, lenR, "sumLR2");
        llvm::Value *termPtr = builder_.CreateGEP(
            /*PointeeType=*/i8Ty,
            /*Ptr=*/buf,
            /*Index=*/sumLR2, "termPtr");
        builder_.CreateStore(llvm::ConstantInt::get(i8Ty, 0), termPtr);

        // 9) Devolver buffer mutable
        return buf;
      }
      break;
    }
    default:
      break;
    }
  }

  // Igualdad/desigualdad genérica (punteros, nullptr, enums)
  if (node->op().type() == TokenType::EQUAL ||
      node->op().type() == TokenType::NOTEQUAL) {
    llvm::ICmpInst::Predicate pred = (node->op().type() == TokenType::EQUAL)
                                         ? llvm::ICmpInst::ICMP_EQ
                                         : llvm::ICmpInst::ICMP_NE;
    return builder_.CreateICmp(pred, L, R, "ptreq");
  }

  // Relacionales genéricas (< <= > >=) para punteros o enums
  if (node->op().type() == TokenType::OPERATOR_SMALLER ||
      node->op().type() == TokenType::SMALLEREQUAL ||
      node->op().type() == TokenType::OPERATOR_GREATER ||
      node->op().type() == TokenType::BIGGEREQUAL) {
    llvm::ICmpInst::Predicate pred;
    switch (node->op().type()) {
    case TokenType::OPERATOR_SMALLER:
      pred = llvm::ICmpInst::ICMP_ULT;
      break;
    case TokenType::SMALLEREQUAL:
      pred = llvm::ICmpInst::ICMP_ULE;
      break;
    case TokenType::OPERATOR_GREATER:
      pred = llvm::ICmpInst::ICMP_UGT;
      break;
    case TokenType::BIGGEREQUAL:
      pred = llvm::ICmpInst::ICMP_UGE;
      break;
    default:
      pred = llvm::ICmpInst::ICMP_EQ;
      break;
    }
    return builder_.CreateICmp(pred, L, R, "relctmp");
  }

  return createError(ERROR_TYPE::TYPE,
                     "operator not supported in AST_BINARY: " +
                         tokenTypeToString(node->op().type()));
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_UNARY *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_UNARY");

  // Generar código del operando
  auto valOrErr = node->value()->accept(*this);
  if (!valOrErr)
    return createError(valOrErr.error());
  llvm::Value *V = *valOrErr;

  // Resolver tipo semántico (desenrollar ConstType)
  auto semTy = node->returnedFromTypeAnalysis();
  if (auto c = std::dynamic_pointer_cast<ConstType>(semTy))
    semTy = c->baseType();

  // Unary sobre BasicType
  if (auto basic = std::dynamic_pointer_cast<BasicType>(semTy)) {
    switch (node->op().type()) {
    // Operador lógico NOT: !v
    case TokenType::OPERATOR_NOT:
      // Para bool: bitwise XOR con 1 producirá not lógico
      return builder_.CreateXor(V, llvm::ConstantInt::get(V->getType(), 1),
                                "nottmp");

    // Negación aritmética: -v
    case TokenType::OPERATOR_SUB:
      if (basic->baseKind() == BasicKind::Int) {
        return builder_.CreateNeg(V, "negtmp");
      } else {
        // float o double
        return builder_.CreateFNeg(V, "fnegtmp");
      }

    // Pre-incremento y pre-decremento: ++v, --v
    case TokenType::INCREMENT:
    case TokenType::DECREMENT: {
      bool isInc = (node->op().type() == TokenType::INCREMENT);
      // Constante 1 del tipo adecuado
      llvm::Value *one = nullptr;
      if (basic->baseKind() == BasicKind::Int) {
        one = llvm::ConstantInt::get(V->getType(), 1);
        return isInc ? builder_.CreateAdd(V, one, "inc")
                     : builder_.CreateSub(V, one, "dec");
      } else {
        // float o double
        one = llvm::ConstantFP::get(V->getType(), 1.0);
        return isInc ? builder_.CreateFAdd(V, one, "finc")
                     : builder_.CreateFSub(V, one, "fdec");
      }
    }

    default:
      break;
    }
  }

  // Unary sobre PointerType
  if (auto ptrType = std::dynamic_pointer_cast<PointerType>(semTy)) {
    switch (node->op().type()) {
    case TokenType::OPERATOR_NOT:
      // v == nullptr
      return builder_.CreateICmpEQ(
          V,
          llvm::ConstantPointerNull::get(
              llvm::cast<llvm::PointerType>(V->getType())),
          "isnull");

    case TokenType::INCREMENT:
    case TokenType::DECREMENT: {
      return createError(
          ERROR_TYPE::TYPE,
          "llvm increment and decrement for not implemented yet");
    }

    default:
      break;
    }
  }

  // Resto de casos no soportados
  return createError(ERROR_TYPE::TYPE,
                     "unsupported unary operator: " +
                         tokenTypeToString(node->op().type()));
}

} // namespace nicole