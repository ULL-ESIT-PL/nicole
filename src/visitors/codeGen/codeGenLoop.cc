#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/loops/nodeDoWhile.h"
#include "../../../inc/parsingAnalysis/ast/loops/nodeForStatement.h"
#include "../../../inc/parsingAnalysis/ast/loops/nodePass.h"
#include "../../../inc/parsingAnalysis/ast/loops/nodeStop.h"
#include "../../../inc/parsingAnalysis/ast/loops/nodeWhileStatement.h"
#include "../../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeWhileStatement *node) const {
  llvm::Function *TheFunction = builder_.GetInsertBlock()->getParent();

  llvm::BasicBlock *CondBB =
      llvm::BasicBlock::Create(*context_, "while.cond", TheFunction);
  llvm::BasicBlock *BodyBB = llvm::BasicBlock::Create(
      *context_, "while.body", TheFunction); // si pongo Thefunction luego no me
                                             // hace falta hacer un insert
  llvm::BasicBlock *EndBB = llvm::BasicBlock::Create(*context_, "while.end");

  // Saltar inmediatamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de condición
  builder_.SetInsertPoint(CondBB);

  // Evaluar la condición
  llvm::Value *condition = node->condition()->accept(this);
  if (!condition) {
    return nullptr; // Error al generar la condición
  }

  // Crear una instrucción condicional para decidir si continuar con el bucle o
  // terminar
  builder_.CreateCondBr(condition, BodyBB, EndBB);

  // Insertar el bloque del cuerpo del bucle
  builder_.SetInsertPoint(BodyBB);

  // Generar el cuerpo del bucle
  node->body()->accept(this);

  // Al finalizar el cuerpo, saltar nuevamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de finalización (salida del bucle)
  TheFunction->insert(TheFunction->end(), EndBB);
  builder_.SetInsertPoint(EndBB);

  // No retorna ningún valor
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeDoWhileStatement *node) const {
  llvm::Function *TheFunction = builder_.GetInsertBlock()->getParent();

  // We invert the secuence of creating with the parameter TheFunction so the
  // body is created first unlike for a while statement
  llvm::BasicBlock *BodyBB =
      llvm::BasicBlock::Create(*context_, "doWhile.body",
                               TheFunction); // si pongo Thefunction luego no me
  // hace falta hacer un insert
  llvm::BasicBlock *CondBB =
      llvm::BasicBlock::Create(*context_, "doWhile.cond", TheFunction);
  llvm::BasicBlock *EndBB = llvm::BasicBlock::Create(*context_, "doWhile.end");

  // Jump to the body
  builder_.CreateBr(BodyBB);

  // Start inserting into the body
  builder_.SetInsertPoint(BodyBB);

  llvm::Value *body{node->body()->accept(this)};

  // Create condition expression
  llvm::Value *condition{node->condition()->accept(this)};

  // If condition is true we jump to body again
  builder_.CreateCondBr(condition, BodyBB, EndBB);

  // Insertar el bloque de finalización (salida del bucle)
  TheFunction->insert(TheFunction->end(), EndBB);
  builder_.SetInsertPoint(EndBB);

  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeForStatement *node) const {
  // Crear los bloques básicos: condición, cuerpo y finalización del while

  auto initiation{node->init()->accept(this)};

  llvm::Function *TheFunction = builder_.GetInsertBlock()->getParent();

  llvm::BasicBlock *CondBB =
      llvm::BasicBlock::Create(*context_, "for.cond", TheFunction);
  llvm::BasicBlock *BodyBB = llvm::BasicBlock::Create(
      *context_, "for.body", TheFunction); // si pongo Thefunction luego no me
                                           // hace falta hacer un insert
  llvm::BasicBlock *EndBB = llvm::BasicBlock::Create(*context_, "for.end");

  // Saltar inmediatamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de condición
  builder_.SetInsertPoint(CondBB);

  // Evaluar la condición
  llvm::Value *condition = node->condition()->accept(this);
  if (!condition) {
    return nullptr; // Error al generar la condición
  }

  // Crear una instrucción condicional para decidir si continuar con el bucle o
  // terminar
  builder_.CreateCondBr(condition, BodyBB, EndBB);

  // Insertar el bloque del cuerpo del bucle
  builder_.SetInsertPoint(BodyBB);

  // Generar el cuerpo del bucle
  node->body()->accept(this);
  node->update()->accept(this);

  // Al finalizar el cuerpo, saltar nuevamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de finalización (salida del bucle)
  TheFunction->insert(TheFunction->end(), EndBB);
  builder_.SetInsertPoint(EndBB);

  // No retorna ningún valor
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStop *node) const {
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodePass *node) const {
  return nullptr;
}

} // namespace nicole