#ifndef NODE_BUILDER_H
#define NODE_BUILDER_H

#include <filesystem>
#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "../types/typeTable.h"
#include "calls/functionCall.h"
#include "calls/structConstructor.h"
#include "calls/variableCall.h"
#include "conditionals/nodeCase.h"
#include "conditionals/nodeIfStatement.h"
#include "conditionals/nodeSwitch.h"
#include "declaration/constDeclaration.h"
#include "declaration/functionTable.h"
#include "declaration/nodeFunDeclaration.h"
#include "declaration/nodeReturn.h"
#include "declaration/paramsDeclaration.h"
#include "declaration/structDeclaration.h"
#include "declaration/varDeclaration.h"
#include "declaration/varReassignment.h"
#include "declaration/varTable.h"
#include "literals/nodeLiteralBool.h"
#include "literals/nodeLiteralChar.h"
#include "literals/nodeLiteralDouble.h"
#include "literals/nodeLiteralFloat.h"
#include "literals/nodeLiteralInt.h"
#include "literals/nodeLiteralString.h"
#include "loops/nodeDoWhile.h"
#include "loops/nodeForStatement.h"
#include "loops/nodePass.h"
#include "loops/nodeStop.h"
#include "loops/nodeWhileStatement.h"
#include "nodeType.h"
#include "operations/nodeBinaryOp.h"
#include "operations/nodeIncrement.h"
#include "operations/nodeUnaryOp.h"
#include "utils/nodeImport.h"
#include "utils/nodePrint.h"

namespace nicole {

// Helper to build nodes
class ASTBuilder {
private:
  ASTBuilder() = delete;

public:
  ~ASTBuilder() = default;

  static std::shared_ptr<NodeLiteralBool>
  createBool(const bool val, std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeLiteralBool>(val, father);
  };

  static std::shared_ptr<NodeLiteralChar>
  createChar(const std::string &val, std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeLiteralChar>(val, father);
  };

  static std::shared_ptr<NodeLiteralFloat>
  createFloat(const float val, std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeLiteralFloat>(val, father);
  };

  static std::shared_ptr<NodeLiteralDouble>
  createDouble(const double val, std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeLiteralDouble>(val, father);
  };

  static std::shared_ptr<NodeLiteralInt>
  createInt(const int val, std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeLiteralInt>(val, father);
  };

  static std::shared_ptr<NodeLiteralString>
  createString(const std::string &val, std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeLiteralString>(val, father);
  };

  static std::shared_ptr<NodeReturn>
  createReturn(std::shared_ptr<Node> expression,
               std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeReturn>(expression, father);
  };

  static std::shared_ptr<NodeBinaryOp>
  createBinOp(std::shared_ptr<Node> left, const TokenType &op,
              std::shared_ptr<Node> right,
              std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeBinaryOp>(left, op, right, father);
  };

  static std::shared_ptr<NodeUnaryOp>
  createUnaryOp(const TokenType &op, std::shared_ptr<Node> expression,
                std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeUnaryOp>(op, expression, father);
  };

  static std::shared_ptr<NodeIncrement>
  createIncrement(const TokenType &op, std::shared_ptr<Node> expression,
                  std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeIncrement>(op, expression, father);
  };

  static std::shared_ptr<NodeStatement>
  createStatement(std::shared_ptr<Node> expression,
                  std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeStatement>(expression, father);
  };

  static std::shared_ptr<NodeStatementList>
  createList(const std::vector<std::shared_ptr<NodeStatement>> &statements,
             std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeStatementList>(statements, father);
  };

  static std::shared_ptr<NodeVariableDeclaration>
  createVarDecl(const std::string &id, std::shared_ptr<GenericType> varType,
                std::shared_ptr<Node> expression,
                std::shared_ptr<VariableTable> currentScope,
                std::shared_ptr<TypeTable> typeTable,
                std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeVariableDeclaration>(
        id, varType, expression, currentScope, typeTable, father);
  };

  static std::shared_ptr<NodeConstDeclaration>
  createConstDecl(const std::string &id, std::shared_ptr<GenericType> varType,
                  std::shared_ptr<Node> expression,
                  std::shared_ptr<VariableTable> currentScope,
                  std::shared_ptr<TypeTable> typeTable,
                  std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeConstDeclaration>(
        id, varType, expression, currentScope, typeTable, father);
  };

  static std::shared_ptr<NodeStructDeclaration>
  createStructDecl(std::shared_ptr<GenericType> idType,
                   std::shared_ptr<NodeStatementList> body,
                   std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeStructDeclaration>(idType, body, father);
  };

  static std::shared_ptr<NodeFunctionDeclaration>
  createFunctDecl(const std::string &id,
                  std::shared_ptr<ParamsDeclaration> params,
                  std::shared_ptr<GenericType> returnType,
                  std::shared_ptr<NodeStatementList> body,
                  std::shared_ptr<VariableTable> currentScope,
                  std::shared_ptr<TypeTable> typeTable,
                  std::shared_ptr<FunctionTable> functionTable,
                  std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeFunctionDeclaration>(
        id, params, returnType, body, currentScope, typeTable, functionTable,
        father);
  };

  static std::shared_ptr<NodeStructConstructor>
  createStructConstr(const std::string &id,
                     std::shared_ptr<NodeStatementList> parameters,
                     std::shared_ptr<TypeTable> typeTable,
                     std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeStructConstructor>(id, parameters, typeTable,
                                                   father);
  };

  static std::shared_ptr<NodeVariableCall>
  createVarCall(const std::string &id,
                std::shared_ptr<VariableTable> currentScope,
                std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeVariableCall>(id, currentScope, father);
  };

  static std::shared_ptr<NodeFunctionCall>
  createFunctCall(const std::string &id,
                  std::vector<std::shared_ptr<Node>> params,
                  std::shared_ptr<VariableTable> variableTable,
                  std::shared_ptr<FunctionTable> functionTable,
                  std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeFunctionCall>(id, params, variableTable,
                                              functionTable, father);
  };

  static std::shared_ptr<NodeVariableReassignment>
  createVarRGT(const std::string &id, std::shared_ptr<Node> expression,
               std::shared_ptr<VariableTable> currentScope,
               std::shared_ptr<TypeTable> typeTable,
               std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeVariableReassignment>(
        id, expression, currentScope, typeTable, father);
  };

  static std::shared_ptr<NodeSwitchStatement>
  createSwitch(std::shared_ptr<Node> match,
               std::vector<std::shared_ptr<NodeCaseStatement>> cases,
               std::shared_ptr<NodeCaseStatement> defaultCase,
               std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeSwitchStatement>(match, cases, defaultCase,
                                                 father);
  };

  static std::shared_ptr<NodeCaseStatement>
  createCase(std::shared_ptr<Node> match,
             std::shared_ptr<NodeStatementList> body,
             std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeCaseStatement>(match, body, father);
  };

  static std::shared_ptr<NodeIfStatement>
  createIf(std::shared_ptr<Node> condition,
           std::shared_ptr<NodeStatementList> body,
           std::shared_ptr<NodeStatementList> elseBody = nullptr,
           std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeIfStatement>(condition, body, elseBody, father);
  };

  static std::shared_ptr<NodeWhileStatement>
  createWhile(std::shared_ptr<Node> condition,
              std::shared_ptr<NodeStatementList> body,
              std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeWhileStatement>(condition, body, father);
  };

  static std::shared_ptr<NodeDoWhileStatement>
  createDoWhile(std::shared_ptr<Node> condition,
                std::shared_ptr<NodeStatementList> body,
                std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeDoWhileStatement>(condition, body, father);
  };

  static std::shared_ptr<NodeForStatement>
  createFor(std::shared_ptr<NodeStatementList> init,
            std::shared_ptr<Node> condition,
            std::shared_ptr<NodeStatementList> update,
            std::shared_ptr<NodeStatementList> body,
            std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeForStatement>(init, condition, update, body,
                                              father);
  };

  static std::shared_ptr<NodeStop>
  createStop(std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeStop>(father);
  };

  static std::shared_ptr<NodePass>
  createPass(std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodePass>(father);
  };

  static std::shared_ptr<NodePrint>
  createPrint(std::vector<std::shared_ptr<Node>> expressions,
              std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodePrint>(expressions, father);
  };

  static std::shared_ptr<NodeImport>
  createImport(const std::filesystem::path &fileName,
               std::shared_ptr<Node> father = nullptr) {
    return std::make_shared<NodeImport>(fileName, father);
  };
};

} // namespace nicole

#endif