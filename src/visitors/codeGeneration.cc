#include "../../inc/visitors/codeGeneration.h"

#include "../../inc/lexicalAnalysis/type.h"
#include "../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../inc/parsingAnalysis/ast/conditionals/NodeIfStatetement.h"
#include "../../inc/parsingAnalysis/ast/declaration/varDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varReassignment.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralBool.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralChar.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralDouble.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralInt.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeBinaryOp.h"
#include "../../inc/parsingAnalysis/ast/statements/statement.h"
#include "../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "../../inc/parsingAnalysis/parsingAlgorithms/tree.h"

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeLiteralBool *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralChar *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralDouble *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  return llvm::ConstantFP::get(*context_, llvm::APFloat(node->value()));
}

llvm::Value *CodeGeneration::visit(const NodeLiteralInt *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_),
                                node->value(), true);
}

llvm::Value *CodeGeneration::visit(const NodeLiteralString *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  llvm::IRBuilder<> builder{*context_};
  llvm::Constant *strConst = llvm::ConstantDataArray::getString(
      *context_, node->value(), /*AddNull=*/true);

  llvm::Value *globalString{builder.CreateGlobalString(
      llvm::StringRef{node->value()}, "str", 0U, currentModule)};
  // Obtener el puntero al string global
  llvm::Value *globalStrPtr = builder.CreatePointerCast(
      globalString, llvm::PointerType::getUnqual(strConst->getType()));

  return globalStrPtr;
}

llvm::Value *CodeGeneration::visit(const NodeBinaryOp *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  Node *left{node->left()};
  Node *right{node->right()};

  llvm::Value *leftEvaluated{left->accept(this, currentEntry, currentModule)};
  llvm::Value *rightEvaluated{right->accept(this, currentEntry, currentModule)};
  if (!leftEvaluated || !rightEvaluated) {
    return nullptr;
  }
  llvm::IRBuilder<> builder(*context_);
  // Convertir tipos si son diferentes (int a double)
  llvm::Type *leftType = leftEvaluated->getType();
  llvm::Type *rightType = rightEvaluated->getType();

  // Si left es int y right es double, convierte left a double
  if (leftType->isIntegerTy() && rightType->isFloatingPointTy()) {
    leftEvaluated =
        builder.CreateSIToFP(leftEvaluated, rightType, "intToDouble");
  }
  // Si left es double y right es int, convierte right a double
  else if (leftType->isFloatingPointTy() && rightType->isIntegerTy()) {
    rightEvaluated =
        builder.CreateSIToFP(rightEvaluated, leftType, "intToDouble");
  }

  // Asegúrate de que ambos operandos tengan el mismo tipo ahora (ambos double o
  // ambos int)
  if (leftEvaluated->getType() != rightEvaluated->getType()) {
    return nullptr; // Error: tipos no compatibles
  }
  // Realiza la operación basada en el tipo del operador
  switch (node->op()) {
  case TokenType::OPERATOR_ADD:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFAdd(leftEvaluated, rightEvaluated, "addtmp");
    } else {
      return builder.CreateAdd(leftEvaluated, rightEvaluated, "addtmp");
    }
  case TokenType::OPERATOR_SUB:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFSub(leftEvaluated, rightEvaluated, "subtmp");
    } else {
      return builder.CreateSub(leftEvaluated, rightEvaluated, "subtmp");
    }
  case TokenType::OPERATOR_MULT:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFMul(leftEvaluated, rightEvaluated, "multmp");
    } else {
      return builder.CreateMul(leftEvaluated, rightEvaluated, "multmp");
    }
  case TokenType::OPERATOR_DIV:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFDiv(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder.CreateSDiv(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::OR:
    if (leftEvaluated->getType()->isIntegerTy()) {
      return builder.CreateOr(leftEvaluated, rightEvaluated, "divmp");
    } 
  case TokenType::AND:
    if (leftEvaluated->getType()->isIntegerTy()) {
      return builder.CreateAnd(leftEvaluated, rightEvaluated, "divmp");
    } 
  case TokenType::EQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFCmpOEQ(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder.CreateICmpEQ(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::NOTEQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFCmpONE(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder.CreateICmpNE(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::OPERATOR_SMALLER:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFCmpOLT(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder.CreateICmpSLT(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::SMALLEREQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFCmpOLE(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder.CreateICmpSLE(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::OPERATOR_GREATER:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFCmpOGT(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder.CreateICmpSGT(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::BIGGEREQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFCmpOGE(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder.CreateICmpSGE(leftEvaluated, rightEvaluated, "divtmp");
    }
  default:
    return nullptr;
  }
}

llvm::Value *CodeGeneration::visit(const NodeStatement *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  // not using expression leads to infinite loop
  return node->expression()->accept(this, currentEntry, currentModule);
}

llvm::Value *CodeGeneration::visit(const NodeVariableDeclaration *node,
                                   llvm::BasicBlock *currentEntry,

                                   llvm::Module *currentModule) const {
  llvm::IRBuilder<> builder{currentEntry}; // Obtener el contexto del módulo

  llvm::Value *value{
      node->expression()->accept(this, currentEntry, currentModule)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable

  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder.CreateStore(value, alloca);
  std::unique_ptr<GenericType> varType(std::move(node->varType()));
  node->table()->addVariable(node->id(), std::move(varType), value, alloca);

  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeVariableCall *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  std::cout << "---------\n" << *node->table() << std::flush;
  return node->table()->variableValue(node->id());
}

llvm::Value *CodeGeneration::visit(const NodeVariableReassignment *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  llvm::IRBuilder<> builder{
      currentEntry}; // Get the context of the current block

  llvm::AllocaInst *varAddress{node->table()->variableAdress(node->id())};
  llvm::Value *newValue{
      node->expression()->accept(this, currentEntry, currentModule)};
  builder.CreateStore(newValue, varAddress);
  node->table()->setVariable(node->id(), newValue);

  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeIfStatement *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  llvm::IRBuilder<> builder{currentEntry};
  llvm::Value *condition = node->condition()->accept(this, currentEntry, currentModule);
  if (!condition) {
    return nullptr;
  }

  llvm::Function *TheFunction = builder.GetInsertBlock()->getParent();
  llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(*context_, "then", TheFunction);
  llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(*context_, "else");
  llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*context_, "ifcont");

  // Crear la instrucción condicional
  builder.CreateCondBr(condition, ThenBB, ElseBB);

  // Insertar el bloque 'then'
  builder.SetInsertPoint(ThenBB);
  node->body()->accept(this, ThenBB, currentModule); // Ejecutar el cuerpo del 'then'
  builder.CreateBr(MergeBB); // Unir con MergeBB

  // Insertar el bloque 'else'
  TheFunction->insert(TheFunction->end(), ElseBB);
  builder.SetInsertPoint(ElseBB);
  node->elseBody()->accept(this, ElseBB, currentModule); // Ejecutar el cuerpo del 'else'
  builder.CreateBr(MergeBB); // Unir con MergeBB

  // Insertar el bloque 'merge'
  TheFunction->insert(TheFunction->end(), MergeBB);
  builder.SetInsertPoint(MergeBB);

  currentEntry = MergeBB;
  // No devolver valor ya que el 'if' solo controla el flujo
  return builder.CreateRetVoid();
}


/*
llvm::Value *CodeGeneration::visit(const NodeIfStatement *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  llvm::IRBuilder<> builder{currentEntry};
  llvm::Value *condition{
      node->condition()->accept(this, currentEntry, currentModule)};
  if (!condition) {
    return nullptr;
  }
  condition = builder.CreateFCmpONE(
      condition, llvm::ConstantFP::get(*context_, llvm::APFloat(0.0)),
      "ifcond");

  llvm::Function *TheFunction = builder.GetInsertBlock()->getParent();

  llvm::BasicBlock *ThenBB =
      llvm::BasicBlock::Create(*context_, "then", TheFunction);
  llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(*context_, "else");
  llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*context_, "ifcont");

  builder.CreateCondBr(condition, ThenBB, ElseBB);

  builder.SetInsertPoint(ThenBB);

  llvm::Value *ThenV = node->body()->accept(this, ThenBB, currentModule);
  if (!ThenV) {
    builder.CreateBr(MergeBB); // Ir directamente a MergeBB
  } else {
    builder.CreateBr(MergeBB); // Ir a MergeBB después del cuerpo de ThenBB
  }

 // builder.CreateBr(MergeBB);

  ThenBB = builder.GetInsertBlock();

  TheFunction->insert(TheFunction->end(), ElseBB);
  builder.SetInsertPoint(ElseBB);

  llvm::Value *ElseV = node->elseBody()->accept(this, ElseBB, currentModule);
  if (!ElseV) {
    builder.CreateBr(MergeBB); // Ir directamente a MergeBB
  } else {
    builder.CreateBr(MergeBB); // Ir a MergeBB después del cuerpo de ElseBB
  }

  // builder.CreateBr(MergeBB);

  ElseBB = builder.GetInsertBlock();

  TheFunction->insert(TheFunction->end(), MergeBB);
  builder.SetInsertPoint(MergeBB);
  llvm::PHINode *PN =
      builder.CreatePHI(llvm::Type::getVoidTy(*context_), 2, "iftmp");

  PN->addIncoming(ThenV, ThenBB);
  PN->addIncoming(ElseV, ElseBB);
  return PN;
}
*/
llvm::Value *CodeGeneration::visit(const NodeStatementList *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  llvm::IRBuilder<> builder{entry_};
  llvm::Value *lastValue{nullptr};
  for (const auto &statement : *node) {
    llvm::Value *value{statement->accept(this, currentEntry, currentModule)};
    if (statement->expression()->type() == NodeType::VAR_DECL ||
        statement->expression()->type() == NodeType::VAR_REG) {
      std::cout << "SKIPPED->>>"
                << nodeTypeToString(statement->expression()->type()) + "\n"
                << std::flush;
      continue;
    }
    if (!value) {
      return nullptr;
    }
    std::cout << "NOT SKIPPED->>>"
              << nodeTypeToString(statement->expression()->type()) + "\n"
              << std::flush;
    lastValue = value;
  }

  return lastValue;
}

llvm::Value *CodeGeneration::visit(const Tree *node,
                                   llvm::BasicBlock *currentEntry,
                                   llvm::Module *currentModule) const {
  llvm::Value *val{node->root()->accept(this, currentEntry, currentModule)};
  llvm::IRBuilder<> builder{currentEntry};
  return nullptr;
}

} // namespace nicole
