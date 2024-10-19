#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/nodeCase.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/nodeIfStatement.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/nodeSwitch.h"
#include "../../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstddef>
#include <vector>

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeSwitchStatement *node) const {
  llvm::Value *match{node->match()->accept(this)};

  llvm::Function *currentFunction = builder_.GetInsertBlock()->getParent();

  std::vector<llvm::BasicBlock *> casesBB{};
  for (const auto caseStatement : *node) {
    casesBB.push_back(
        llvm::BasicBlock::Create(*context_, "case", currentFunction));
  }

  llvm::BasicBlock *defaultBlock{nullptr};

  llvm::BasicBlock *endBB =
      llvm::BasicBlock::Create(*context_, "end", currentFunction);

  // Crear la instrucción switch
  llvm::SwitchInst *switchInst{
      builder_.CreateSwitch(match, endBB, casesBB.size())};

  // Añadir casos
  auto cases{node->cases()};
  for (size_t i{0}; i < casesBB.size(); ++i) {
    llvm::Value *caseMatch{nullptr};
    // checks if a case is a const variable or a literal
    if (cases[i]->match()->type() == NodeType::CALL_VAR) {
      const auto varCall{
          dynamic_cast<const NodeVariableCall *>(cases[i]->match())};
      if (varCall->table()->isConst(varCall->id())) {
        caseMatch = varCall->table()->variableValue(varCall->id());
      } else {
        const std::string strErr{"Error: the variable " + varCall->id() +
                                 " is not const"};
        llvm::report_fatal_error(strErr.c_str());
      }
    } else {
      caseMatch = cases[i]->match()->accept(this);
    }
    // comprobar tipo de caseMatch con match
    // FALTA COMRPOBAR CUANDO CONST VIENE DADO POR UNA SUMA 0 ASSIGNACION POR EJEMPLO
    if (auto *constInt = llvm::dyn_cast<llvm::ConstantInt>(caseMatch)) {
      switchInst->addCase(constInt, casesBB[i]);
    } else {
      llvm::errs() << "Error: caseMatch no es un valor constante entero.\n";
    }
    builder_.SetInsertPoint(casesBB[i]);
    cases[i]->accept(this);
    builder_.CreateBr(endBB);
  }

  if (node->hasDefault()) {
    defaultBlock =
        llvm::BasicBlock::Create(*context_, "default", currentFunction, endBB);
    switchInst->setDefaultDest(defaultBlock);
    builder_.SetInsertPoint(defaultBlock);
    node->defaultCase()->accept(this);
    builder_.CreateBr(endBB);
  }

  builder_.SetInsertPoint(endBB);
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeCaseStatement *node) const {
  // llvm::report_fatal_error(std::to_string(node->body()->statements().size()).c_str());
  node->body()->accept(this);
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