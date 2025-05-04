#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/conditionals/ast_case.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_default.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_elseIf.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_if.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_switch.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_ternary.h"
#include <cstdint>

namespace nicole {

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IF");
  }
  auto condOrErr = node->condition()->accept(*this);
  if (!condOrErr)
    return createError(condOrErr.error());
  llvm::Value *condVal = *condOrErr; // debe ser i1

  // preparar bloques
  llvm::Function *parent = builder_.GetInsertBlock()->getParent();
  std::string id = std::to_string(node->nodeId());
  auto *thenBB = llvm::BasicBlock::Create(context_, "if_then" + id, parent);
  auto *elseBB = llvm::BasicBlock::Create(context_, "if_else" + id, parent);
  auto *mergeBB = llvm::BasicBlock::Create(context_, "if_merge" + id, parent);
  currentMergeBlock_ = mergeBB;

  // salto condicional
  builder_.CreateCondBr(condVal, thenBB, elseBB);

  // THEN block
  builder_.SetInsertPoint(thenBB);
  if (auto thenOrErr = node->body()->accept(*this); !thenOrErr)
    return createError(thenOrErr.error());
  if (!builder_.GetInsertBlock()->getTerminator())
    builder_.CreateBr(mergeBB);

  // ELSE / ELSE IF chain
  builder_.SetInsertPoint(elseBB);
  if (!node->elseIf().empty() || node->elseBody()) {
    // anidar else-if
    for (auto &elif : node->elseIf()) {
      // cada AST_ELSE_IF visitor generará su propio codegen,
      // creando bloques nuevos y saltos a mergeBB o al siguiente else
      if (auto elifOrErr = elif->accept(*this); !elifOrErr)
        return createError(elifOrErr.error());
      // al salir de cada elif, el builder_ estará apuntando al bloque
      // siguiente (o al merge) listo para el próximo elif / else
    }
    // else final
    if (node->elseBody()) {
      if (auto elseOrErr = node->elseBody()->accept(*this); !elseOrErr)
        return createError(elseOrErr.error());
      if (!builder_.GetInsertBlock()->getTerminator())
        builder_.CreateBr(mergeBB);
    }
  } else {
    // no hay else, saltar directamente a merge
    if (!builder_.GetInsertBlock()->getTerminator())
      builder_.CreateBr(mergeBB);
  }

  // merge
  builder_.SetInsertPoint(mergeBB);
  return nullptr; // el if no produce un value útil por sí solo
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_ELSE_IF *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ELSE_IF");
  auto condOrErr = node->condition()->accept(*this);
  if (!condOrErr)
    return createError(condOrErr.error());
  llvm::Value *condVal = *condOrErr;

  // Crear bloques for this else-if
  llvm::Function *parent = builder_.GetInsertBlock()->getParent();
  std::string id = std::to_string(node->nodeId());
  auto *thenBB = llvm::BasicBlock::Create(context_, "elif_then" + id, parent);
  auto *nextBB = llvm::BasicBlock::Create(context_, "elif_next" + id, parent);

  // CondBr al then o al siguiente (que puede ser otro elif o merge)
  llvm::BasicBlock *mergeBB = currentMergeBlock_;
  builder_.CreateCondBr(condVal, thenBB, nextBB);

  // THEN de este elif
  builder_.SetInsertPoint(thenBB);
  if (auto r = node->body()->accept(*this); !r)
    return createError(r.error());
  if (!builder_.GetInsertBlock()->getTerminator())
    builder_.CreateBr(mergeBB);

  // Continuar en el siguiente bloque (nextBB)
  builder_.SetInsertPoint(nextBB);
  return nullptr;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_SWITCH *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SWITCH");

  // Evaluar la condición
  auto condOrErr = node->condition()->accept(*this);
  if (!condOrErr)
    return createError(condOrErr.error());
  llvm::Value *condVal = *condOrErr;

  // Preparar bloques y switchInst
  llvm::Function *parent = builder_.GetInsertBlock()->getParent();
  std::string id = std::to_string(node->nodeId());

  // Bloque final (merge)
  llvm::BasicBlock *mergeBB =
      llvm::BasicBlock::Create(context_, "switch_merge" + id, parent);

  // Empujar destino de break para 'stop' dentro del switch
  breakTargets_.push(mergeBB);

  // Bloque por defecto (o salto directo a merge si no hay default)
  llvm::BasicBlock *defaultBB =
      node->defaultCase()
          ? llvm::BasicBlock::Create(context_, "switch_default" + id, parent)
          : mergeBB;

  // Crear la instrucción switch
  llvm::SwitchInst *sw = builder_.CreateSwitch(
      condVal, defaultBB, static_cast<uint32_t>(node->cases().size()));

  // Recorrer cada case: crear bloque, registrarlo y emitir body
  for (auto &caseNode : node->cases()) {
    // Evaluar el literal del case
    auto litOrErr = caseNode->match()->accept(*this);
    if (!litOrErr)
      return createError(litOrErr.error());
    auto *litVal = llvm::dyn_cast<llvm::ConstantInt>(*litOrErr);
    if (!litVal)
      return createError(ERROR_TYPE::TYPE,
                         "switch case match is not a constant integer");

    // Crear el BasicBlock para este case
    auto caseNum = std::to_string(litVal->getZExtValue());
    llvm::BasicBlock *caseBB = llvm::BasicBlock::Create(
        context_, "switch_case" + id + "_" + caseNum, parent);

    // Registrar el case en el switch
    sw->addCase(litVal, caseBB);

    // Emitir el cuerpo del case
    builder_.SetInsertPoint(caseBB);
    if (auto r = caseNode->body()->accept(*this); !r)
      return createError(r.error());
    if (!builder_.GetInsertBlock()->getTerminator())
      builder_.CreateBr(mergeBB);
  }

  // Emitir default si existe
  if (node->defaultCase()) {
    builder_.SetInsertPoint(defaultBB);
    if (auto r = node->defaultCase()->body()->accept(*this); !r)
      return createError(r.error());
    if (!builder_.GetInsertBlock()->getTerminator())
      builder_.CreateBr(mergeBB);
  }

  // Popear la pila de breaks al salir del switch
  breakTargets_.pop();

  // Continuar en merge
  builder_.SetInsertPoint(mergeBB);
  return nullptr;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_CASE *node) const noexcept {
  // Ya manejado en visit(AST_SWITCH*); aquí solo por completitud
  return node->body()->accept(*this);
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_DEFAULT *node) const noexcept {
  // Similarmente, su cuerpo ya se generó en visit(AST_SWITCH*)
  return node->body()->accept(*this);
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_TERNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_TERNARY");
  }
  auto condOrErr = node->condition()->accept(*this);
  if (!condOrErr)
    return createError(condOrErr.error());
  llvm::Value *condVal = *condOrErr; // debe ser un i1

  // Crear bloques
  llvm::Function *parent = builder_.GetInsertBlock()->getParent();
  std::string id = std::to_string(node->nodeId());
  auto *thenBB = llvm::BasicBlock::Create(context_, "tern_then" + id, parent);
  auto *elseBB = llvm::BasicBlock::Create(context_, "tern_else" + id, parent);
  auto *mergeBB = llvm::BasicBlock::Create(context_, "tern_merge" + id, parent);

  // Branch condicional
  builder_.CreateCondBr(condVal, thenBB, elseBB);

  // THEN block
  builder_.SetInsertPoint(thenBB);
  auto thenOrErr = node->first()->accept(*this);
  if (!thenOrErr)
    return createError(thenOrErr.error());
  llvm::Value *thenVal = *thenOrErr;
  if (!builder_.GetInsertBlock()->getTerminator())
    builder_.CreateBr(mergeBB); // saltar a merge al final

  // ELSE block
  builder_.SetInsertPoint(elseBB);
  auto elseOrErr = node->second()->accept(*this);
  if (!elseOrErr)
    return createError(elseOrErr.error());
  llvm::Value *elseVal = *elseOrErr;
  if (!builder_.GetInsertBlock()->getTerminator())
    builder_.CreateBr(mergeBB);

  // Merge block y PHI node
  builder_.SetInsertPoint(mergeBB);
  // El tipo de los valores thenVal y elseVal debe ser el mismo
  llvm::PHINode *phi =
      builder_.CreatePHI(thenVal->getType(), 2, "tern_phi" + id);
  phi->addIncoming(thenVal, thenBB);
  phi->addIncoming(elseVal, elseBB);

  return phi;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_CONDITION *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CONDITION");
  }
  return node->condition()->accept(*this);
}

} // namespace nicole