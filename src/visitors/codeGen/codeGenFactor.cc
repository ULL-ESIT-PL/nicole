#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/lexicalAnalysis/type.h"
#include "../../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../../inc/parsingAnalysis/ast/literals/nodeLiteralBool.h"
#include "../../../inc/parsingAnalysis/ast/literals/nodeLiteralChar.h"
#include "../../../inc/parsingAnalysis/ast/literals/nodeLiteralDouble.h"
#include "../../../inc/parsingAnalysis/ast/literals/nodeLiteralFloat.h"
#include "../../../inc/parsingAnalysis/ast/literals/nodeLiteralInt.h"
#include "../../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"
#include "../../../inc/parsingAnalysis/ast/operations/nodeIncrement.h"
#include "../../../inc/parsingAnalysis/ast/operations/nodeUnaryOp.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeLiteralBool *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralChar *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralFloat *node) const {
  return llvm::ConstantFP::get(llvm::Type::getFloatTy(*context_),
                               llvm::APFloat(node->value()));
}

llvm::Value *CodeGeneration::visit(const NodeLiteralDouble *node) const {
  return llvm::ConstantFP::get(llvm::Type::getDoubleTy(*context_),
                               llvm::APFloat(node->value()));
}

llvm::Value *CodeGeneration::visit(const NodeLiteralInt *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_),
                                node->value(), true);
}

llvm::Value *CodeGeneration::visit(const NodeLiteralString *node) const {
  llvm::Constant *strConst = llvm::ConstantDataArray::getString(
      *context_, node->value(), /*AddNull=*/true);

  llvm::Value *globalString{
      builder_.CreateGlobalString(llvm::StringRef{node->value()}, "str", 0U)};
  // Obtener el puntero al string global
  llvm::Value *globalStrPtr = builder_.CreatePointerCast(
      globalString, llvm::PointerType::getUnqual(strConst->getType()));

  return globalStrPtr;
}

llvm::Value *CodeGeneration::visit(const NodeUnaryOp *node) const {
  const Node *expression{node->expression()};

  llvm::Value *expressionEvaluated{expression->accept(this)};
  if (!expressionEvaluated) {
    return nullptr;
  }
  llvm::Type *expressionType = expressionEvaluated->getType();
  switch (node->op()) {
  case TokenType::OPERATOR_NOT: {
    if (expressionType->isIntegerTy(1)) {
      return builder_.CreateNot(expressionEvaluated, "notTemp");
    } else {
      llvm::report_fatal_error("Can only use not operator with booleans");
    }
  }
  case TokenType::OPERATOR_SUB: {
    if (expressionType->isIntegerTy()) {
      return builder_.CreateNeg(expressionEvaluated, "arithNegTemp");
    } else if (expressionType->isDoubleTy()) {
      return builder_.CreateFNeg(expressionEvaluated, "arithNegTemp");
    } else {
      llvm::report_fatal_error(
          "Can only use arithmetic negation operator with int or double");
    }
  }
  default:
    llvm::llvm_unreachable_internal("Operator not supported");
  }
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeIncrement *node) const {
  const Node *expression{node->expression()};
  if (expression->type() != NodeType::CALL_VAR) {
    llvm::report_fatal_error(
        "Can only use ++ operator with variables of type int");
  }
  const NodeVariableCall *castedExpression{
      dynamic_cast<const NodeVariableCall *>(expression)};
  llvm::Value *expressionEvaluated{expression->accept(this)};
  if (!expressionEvaluated) {
    return nullptr;
  }
  llvm::Type *expressionType = expressionEvaluated->getType();
  if (!expressionType->isIntegerTy(32)) {
    llvm::report_fatal_error(
        "Can only use ++ operator with variables of type int");
  }
  llvm::Value *one{
      llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 1)};
  switch (node->op()) {
  case TokenType::INCREMENT: {
    llvm::Value *newValue{
        builder_.CreateAdd(expressionEvaluated, one, "increTemp")};
    llvm::AllocaInst *varAddress{
        castedExpression->table()->variableAddress(castedExpression->id())};
    builder_.CreateStore(newValue, varAddress);
    castedExpression->table()->setVariable(castedExpression->id(), newValue);
    return newValue;
  }
  case TokenType::DECREMENT: {
    llvm::Value *newValue{
        builder_.CreateSub(expressionEvaluated, one, "decreTemp")};
    llvm::AllocaInst *varAddress{
        castedExpression->table()->variableAddress(castedExpression->id())};
    builder_.CreateStore(newValue, varAddress);
    castedExpression->table()->setVariable(castedExpression->id(), newValue);
    return newValue;
  }
  default:
    llvm::llvm_unreachable_internal("Operator not supported");
  }
  return nullptr;
}

} // namespace nicole