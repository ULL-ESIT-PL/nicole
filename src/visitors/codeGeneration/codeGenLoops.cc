#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/loops/ast_doWhile.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_for.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_pass.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_stop.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_while.h"
#include <cstddef>
#include <memory>
#include <variant>

namespace nicole {

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_WHILE");
  }
  auto condOrErr = node->condition()->accept(*this);
  if (!condOrErr)
    return createError(condOrErr.error());
  llvm::Value *condVal = *condOrErr; // debe ser un i1

  // 2) Preparar los bloques
  llvm::Function *parent = builder_.GetInsertBlock()->getParent();
  std::string id = std::to_string(node->nodeId());
  // Bloque para la condición
  llvm::BasicBlock *condBB =
      llvm::BasicBlock::Create(context_, "while_cond" + id, parent);
  // Bloque para el cuerpo
  llvm::BasicBlock *bodyBB =
      llvm::BasicBlock::Create(context_, "while_body" + id, parent);
  // Bloque de salida
  llvm::BasicBlock *mergeBB =
      llvm::BasicBlock::Create(context_, "while_merge" + id, parent);

  // Salto inicial a la condición
  builder_.CreateBr(condBB);

  // Condición: branch condicional
  builder_.SetInsertPoint(condBB);
  builder_.CreateCondBr(condVal, bodyBB, mergeBB);

  // Empujar destinos
  continueTargets_.push(condBB);
  breakTargets_.push(mergeBB);

  // Cuerpo del while
  builder_.SetInsertPoint(bodyBB);
  if (auto bodyOrErr = node->body()->accept(*this); !bodyOrErr)
    return createError(bodyOrErr.error());
  // Si no ya hay terminador (ret, break, continue...), saltamos de vuelta
  if (!builder_.GetInsertBlock()->getTerminator())
    builder_.CreateBr(condBB);

  // Limpiar pilas al salir
  continueTargets_.pop();
  breakTargets_.pop();

  // Punto de continuación fuera del bucle
  builder_.SetInsertPoint(mergeBB);
  return nullptr;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FOR");
  }
  // Emitir las inicializaciones
  for (const auto &expr : node->init()) {
    if (auto initOrErr = expr->accept(*this); !initOrErr)
      return createError(initOrErr.error());
    // No comprobamos terminator aquí: 'for' init nunca contiene return
  }

  // Crear los bloques: cond, body, update, merge
  llvm::Function *parent = builder_.GetInsertBlock()->getParent();
  std::string id = std::to_string(node->nodeId());
  auto *condBB = llvm::BasicBlock::Create(context_, "for_cond" + id, parent);
  auto *bodyBB = llvm::BasicBlock::Create(context_, "for_body" + id, parent);
  auto *updateBB =
      llvm::BasicBlock::Create(context_, "for_update" + id, parent);
  auto *mergeBB = llvm::BasicBlock::Create(context_, "for_merge" + id, parent);

  // Primer salto a la condición
  builder_.CreateBr(condBB);

  // Bloque de condición
  builder_.SetInsertPoint(condBB);
  auto condOrErr = node->condition()->accept(*this);
  if (!condOrErr)
    return createError(condOrErr.error());
  llvm::Value *condVal = *condOrErr; // debe ser i1
  builder_.CreateCondBr(condVal, bodyBB, mergeBB);

  // Empujar destinos
  continueTargets_.push(condBB);
  breakTargets_.push(mergeBB);

  // Bloque del cuerpo
  builder_.SetInsertPoint(bodyBB);
  if (auto bodyOrErr = node->body()->accept(*this); !bodyOrErr)
    return createError(bodyOrErr.error());
  // tras el body, si no hay terminador (return/break), saltar a update
  if (!builder_.GetInsertBlock()->getTerminator())
    builder_.CreateBr(updateBB);

  // Bloque de update
  builder_.SetInsertPoint(updateBB);
  for (const auto &expr : node->update()) {
    if (auto updOrErr = expr->accept(*this); !updOrErr)
      return createError(updOrErr.error());
  }
  // tras actualizar, si no hay terminador, regresar a condBB
  if (!builder_.GetInsertBlock()->getTerminator())
    builder_.CreateBr(condBB);

  // Limpiar pilas al salir
  continueTargets_.pop();
  breakTargets_.pop();

  // Continuar en mergeBB
  builder_.SetInsertPoint(mergeBB);
  return nullptr;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DO_WHILE");
  }
  // 1) Preparar bloques
  llvm::Function *parent = builder_.GetInsertBlock()->getParent();
  std::string id = std::to_string(node->nodeId());
  // Bloque para el cuerpo (se ejecuta primero)
  llvm::BasicBlock *bodyBB =
      llvm::BasicBlock::Create(context_, "dowhile_body" + id, parent);
  // Bloque para la condición
  llvm::BasicBlock *condBB =
      llvm::BasicBlock::Create(context_, "dowhile_cond" + id, parent);
  // Bloque de salida
  llvm::BasicBlock *mergeBB =
      llvm::BasicBlock::Create(context_, "dowhile_merge" + id, parent);

  // Salto inicial al cuerpo
  builder_.CreateBr(bodyBB);

  // Empujar destinos
  continueTargets_.push(condBB);
  breakTargets_.push(mergeBB);

  // Body block
  builder_.SetInsertPoint(bodyBB);
  if (auto bodyOrErr = node->body()->accept(*this); !bodyOrErr)
    return createError(bodyOrErr.error());
  // Si no hay terminador (p.ej. return), saltar a condBB
  if (!builder_.GetInsertBlock()->getTerminator())
    builder_.CreateBr(condBB);

  // Condición
  builder_.SetInsertPoint(condBB);
  auto condOrErr = node->condition()->accept(*this);
  if (!condOrErr)
    return createError(condOrErr.error());
  llvm::Value *condVal = *condOrErr; // debe ser i1
  // Branch según la condición: true → body, false → merge
  builder_.CreateCondBr(condVal, bodyBB, mergeBB);

  // Limpiar pilas al salir
  continueTargets_.pop();
  breakTargets_.pop();

  // Continuar en merge
  builder_.SetInsertPoint(mergeBB);
  return nullptr;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PASS");
  }
  if (continueTargets_.empty())
    return createError(ERROR_TYPE::UNKOWN_ARGUMENT,
                       "continue used outside of loop/switch");
  // Saltar al destino de continue del bucle actual
  llvm::BasicBlock *contBB = continueTargets_.top();
  builder_.CreateBr(
      contBB); // unconditional branch :contentReference[oaicite:0]{index=0}
  // A partir de aquí, cualquier código subsiguiente no se ejecuta en este
  // bloque
  return nullptr;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STOP");
  }
  if (breakTargets_.empty())
    return createError(ERROR_TYPE::UNKOWN_ARGUMENT,
                       "break used outside of loop/switch");
  // Saltar al destino de break del bucle o switch actual
  llvm::BasicBlock *brkBB = breakTargets_.top();
  builder_.CreateBr(
      brkBB); // unconditional branch :contentReference[oaicite:2]{index=2}
  return nullptr;
}

} // namespace nicole