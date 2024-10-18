#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/conditionals/nodeCase.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/nodeIfStatement.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/nodeSwitch.h"
#include "../../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeSwitchStatement *node) const {
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeCaseStatement *node) const {
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeIfStatement *node) const {
  llvm::Value *condition = node->condition()->accept(this);
  if (!condition) {
    return nullptr;
  }

  llvm::Function *TheFunction = builder_.GetInsertBlock()->getParent();
  llvm::BasicBlock *ThenBB =
      llvm::BasicBlock::Create(*context_, "then", TheFunction);
  llvm::BasicBlock *ElseBB = nullptr;
  llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*context_, "ifcont");

  if (node->hasElse()) {
    // Crear la instrucción condicional con 'then' y 'else'
    ElseBB = llvm::BasicBlock::Create(*context_, "else");
    builder_.CreateCondBr(condition, ThenBB, ElseBB);
  } else {
    // Crear la instrucción condicional con solo 'then'
    builder_.CreateCondBr(condition, ThenBB, MergeBB);
  }

  // Insertar el bloque 'then'
  builder_.SetInsertPoint(ThenBB);
  node->body()->accept(this); // Ejecutar el cuerpo del 'then'
  // to avoid branching if has return
  bool ifHasReturn{false};
  for (const auto &inst : *ThenBB) {
    if (llvm::isa<llvm::ReturnInst>(inst)) {
      ifHasReturn = true;
    }
  }
  if (!ifHasReturn) {
    builder_.CreateBr(MergeBB); // Unir con MergeBB
  }
  if (ElseBB) {
    // Insertar el bloque 'else'
    TheFunction->insert(TheFunction->end(), ElseBB);
    builder_.SetInsertPoint(ElseBB);
    node->elseBody()->accept(this); // Ejecutar el cuerpo del 'else'
    // to avoid branching if has return
    bool elseHasReturn{false};
    for (const auto &inst : *ElseBB) {
      if (llvm::isa<llvm::ReturnInst>(inst)) {
        elseHasReturn = true;
      }
    }
    if (!elseHasReturn) {
      builder_.CreateBr(MergeBB); // Unir con MergeBB
    }
  }

  // Insertar el bloque 'merge'
  TheFunction->insert(TheFunction->end(), MergeBB);
  builder_.SetInsertPoint(MergeBB);
  // No devolver valor ya que el 'if' solo controla el flujo
  return nullptr;
}

} // namespace nicole