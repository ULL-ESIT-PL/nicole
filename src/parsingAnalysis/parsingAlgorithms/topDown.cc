#include "../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <filesystem>
#include <memory>

namespace nicole {

std::shared_ptr<Tree> TopDown::parse(const std::filesystem::path &path) const {
  tkStream_ = lexer_.analyze(path, false);
  parsedFiles_.insert(path);
  globalScope_ = TBBuilder::createScope(nullptr);
  typeTable_ = TBBuilder::createTypeTB();
  functionTable_ = TBBuilder::createFunctTB();
  root_ = parseStart();

  return std::make_shared<Tree>(root_);
}

std::shared_ptr<NodeStatementList> TopDown::parseStart() const {
  std::vector<std::shared_ptr<NodeStatement>> gloablScopeStatements{};
  std::shared_ptr<Node> father{nullptr};
  while (tkStream_.currentPos() < tkStream_.size()) {
    gloablScopeStatements.push_back(parseStatement(globalScope_, father));
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
      tkStream_.eat();
    }
  }

  return ASTBuilder::createList(gloablScopeStatements);
}

std::shared_ptr<NodeStatementList>
TopDown::parseBody(std::shared_ptr<VariableTable> &bodyScope,
                   std::shared_ptr<Node> father) const {
  if (tkStream_.current().type() == TokenType::LB) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left bracket, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  std::vector<std::shared_ptr<NodeStatement>> body;
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::RB)) {
    body.push_back(parseStatement(bodyScope, father));
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
      tkStream_.eat();
    }
  }
  tkStream_.eat(); // Consume el token "}"
  return ASTBuilder::createList(body);
}

std::shared_ptr<NodeStatementList>
TopDown::parseComma(std::shared_ptr<VariableTable> &currentScope,
                    std::shared_ptr<Node> father) const {
  std::vector<std::shared_ptr<NodeStatement>> body;
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    body.push_back(
        ASTBuilder::createStatement(parseVarDeclaration(currentScope, father)));
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::COMMA)) {
      tkStream_.eat();
    } else {
      break;
    }
  }
  return ASTBuilder::createList(body);
}

std::shared_ptr<ParamsDeclaration>
TopDown::parseParams(std::shared_ptr<VariableTable> &currentScope,
                     std::shared_ptr<Node> father) const {
  std::vector<std::pair<std::string, std::shared_ptr<GenericType>>> params{};
  if (tkStream_.current().type() == TokenType::RP) {
    return std::make_shared<ParamsDeclaration>(params, currentScope);
  }
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    auto token{tkStream_.current()};
    std::string id{};
    std::shared_ptr<GenericType> type{nullptr};
    if (token.type() == TokenType::ID) {
      id = token.raw();
      tkStream_.eat();
    } else {
      const std::string strErr{"Expected parameter, found: " + token.raw() +
                               " at " + token.locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
    if (tkStream_.current().type() == TokenType::DOTDOT) {
      tkStream_.eat();
    } else {
      const std::string strErr{
          "Expected :, found: " + tkStream_.current().raw() + " at " +
          tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
    token = tkStream_.current();
    if (token.type() == TokenType::ID) {
      type = typeTable_->type(token.raw());
      tkStream_.eat();
    } else {
      const std::string strErr{"Expected type, found: " + token.raw() + " at " +
                               token.locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
    params.push_back({id, type});
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::COMMA)) {
      tkStream_.eat();
    } else {
      break;
    }
  }
  return std::make_shared<ParamsDeclaration>(params, currentScope);
}

std::vector<std::shared_ptr<Node>>
TopDown::parseParamsCall(std::shared_ptr<VariableTable> &currentScope,
                         std::shared_ptr<Node> father) const {
  std::vector<std::shared_ptr<Node>> params{};
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    params.push_back(parseLogicalOr(currentScope, father));
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::COMMA)) {
      tkStream_.eat();
    } else {
      break;
    }
  }
  return params;
}

std::shared_ptr<NodeStatement>
TopDown::parseStatement(std::shared_ptr<VariableTable> currentScope,
                        std::shared_ptr<Node> father) const {
  switch (tkStream_.current().type()) {
  case TokenType::IF: {
    return ASTBuilder::createStatement(parseIfStatement(currentScope, father),
                                       father);
  }
  case TokenType::WHILE: {
    return ASTBuilder::createStatement(
        parseWhileStatement(currentScope, father), father);
  }
  case TokenType::FOR: {
    return ASTBuilder::createStatement(parseForStatement(currentScope, father),
                                       father);
  }
  case TokenType::PRINT: {
    return ASTBuilder::createStatement(
        parsePrintStatement(currentScope, father), father);
  }
  case TokenType::STRUCT: {
    return ASTBuilder::createStatement(
        parseStructDeclaration(currentScope, father), father);
  }
  case TokenType::FUNCTION: {
    return ASTBuilder::createStatement(
        parseFunctionDeclaration(currentScope, father), father);
  }
  case TokenType::RETURN: {
    return ASTBuilder::createStatement(parseReturn(currentScope, father),
                                       father);
  }
  case TokenType::IMPORT: {
    return ASTBuilder::createStatement(parseImport(currentScope, father),
                                       father);
  }
  default: {
    return ASTBuilder::createStatement(
        parseVarDeclaration(currentScope, father), father);
  }
  }
}

std::shared_ptr<NodeIfStatement>
TopDown::parseIfStatement(std::shared_ptr<VariableTable> currentScope,
                          std::shared_ptr<Node> father) const {
  tkStream_.eat();
  if (tkStream_.current().type() == TokenType::LP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::RP) {
    const std::string strErr{"Error: empty if condition, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto condition{parseLogicalOr(currentScope, father)};

  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto ifScope{TBBuilder::createScope(currentScope)};
  auto ifBody{parseBody(ifScope, father)};
  std::shared_ptr<NodeStatementList> elseBody{nullptr};
  if (tkStream_.currentPos() < tkStream_.size() and
      tkStream_.current().type() == TokenType::ELSE) {
    tkStream_.eat();
    auto elseScope{TBBuilder::createScope(currentScope)};
    elseBody = parseBody(elseScope, father);
  }
  return ASTBuilder::createIf(condition, ifBody, elseBody);
}

std::shared_ptr<NodeWhileStatement>
TopDown::parseWhileStatement(std::shared_ptr<VariableTable> currentScope,
                             std::shared_ptr<Node> father) const {
  tkStream_.eat();
  if (tkStream_.current().type() == TokenType::LP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::RP) {
    const std::string strErr{"Error: empty while condition, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto condition{parseLogicalOr(currentScope, father)};

  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto whileScope{TBBuilder::createScope(currentScope)};
  auto whileBody{parseBody(whileScope, father)};
  return ASTBuilder::createWhile(condition, whileBody);
}

std::shared_ptr<NodeForStatement>
TopDown::parseForStatement(std::shared_ptr<VariableTable> currentScope,
                           std::shared_ptr<Node> father) const {
  tkStream_.eat();
  if (tkStream_.current().type() == TokenType::LP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::RP) {
    const std::string strErr{"Error: empty for, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto forScope{TBBuilder::createScope(currentScope)};
  auto init{parseComma(forScope, father)};
  if (tkStream_.current().type() == TokenType::SEMICOLON) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing \";\" to separate init from "
                             "condition in for statement, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto condition{parseLogicalOr(forScope, father)};
  if (tkStream_.current().type() == TokenType::SEMICOLON) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing \";\" to separate condition from "
                             "update in for statement, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto update{parseComma(forScope, father)};
  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto body{parseBody(forScope, father)};
  return ASTBuilder::createFor(init, condition, update, body);
}

std::shared_ptr<NodePrint>
TopDown::parsePrintStatement(std::shared_ptr<VariableTable> currentScope,
                             std::shared_ptr<Node> father) const {
  tkStream_.eat();
  if (tkStream_.current().type() == TokenType::LP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::RP) {
    const std::string strErr{"Error: empty print, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto expressions{parseParamsCall(currentScope, father)};

  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  return ASTBuilder::createPrint(expressions);
}

std::shared_ptr<NodeStructDeclaration>
TopDown::parseStructDeclaration(std::shared_ptr<VariableTable> currentScope,
                                std::shared_ptr<Node> father) const {
  tkStream_.eat();
  auto token{tkStream_.current()};
  std::shared_ptr<NodeStatementList> body{nullptr};
  std::shared_ptr<GenericType> idType{nullptr};
  if (token.type() == TokenType::ID) {
    const std::string idTypeStr{token.raw()};
    idType = std::make_shared<UserType>(idTypeStr);
    typeTable_->addType(idType);
    tkStream_.eat();
    if (tkStream_.current().type() == TokenType::LB) {
      auto structScope{TBBuilder::createScope(nullptr)};
      body = parseBody(structScope, father);
    } else {
      const std::string strErr{"Error missing left { of " + idTypeStr + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }
  return ASTBuilder::createStructDecl(idType, body);
}

std::shared_ptr<NodeFunctionDeclaration>
TopDown::parseFunctionDeclaration(std::shared_ptr<VariableTable> currentScope,
                                  std::shared_ptr<Node> father) const {
  tkStream_.eat();
  auto token{tkStream_.current()};
  std::string id{};
  if (token.type() == TokenType::ID) {
    id = token.raw();
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing function id after def, found" +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::LP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing left ( for function " + id +
                             " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto funScope{TBBuilder::createScope(nullptr)};
  std::shared_ptr<ParamsDeclaration> params{parseParams(funScope, father)};
  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing right ) for function " + id +
                             " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::DOTDOT) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing \':\' after right ) for function " +
                             id + " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  token = tkStream_.current();
  std::shared_ptr<GenericType> returnType{nullptr};
  if (token.type() == TokenType::ID) {
    returnType = typeTable_->type(token.raw());
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing return type for the function " +
                             id + " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  std::shared_ptr<NodeStatementList> body{parseBody(funScope, father)};
  return ASTBuilder::createFunctDecl(id, params, returnType, body, funScope,
                                     typeTable_, functionTable_);
}

std::shared_ptr<NodeReturn>
TopDown::parseReturn(std::shared_ptr<VariableTable> &currentScope,
                     std::shared_ptr<Node> father) const {
  tkStream_.eat();
  if (tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    return ASTBuilder::createReturn(nullptr);
  }
  return ASTBuilder::createReturn(parseLogicalOr(currentScope, father));
}

std::shared_ptr<NodeImport>
TopDown::parseImport(std::shared_ptr<VariableTable> &currentScope,
                     std::shared_ptr<Node> father) const {
  tkStream_.eat();
  std::filesystem::path fileName{""};
  if (tkStream_.isCurrentTokenType(TokenType::STRING)) {
    const auto raw{tkStream_.current().raw()};
    fileName = "../test/" + raw.substr(1, raw.size() - 2);
  } else {
    const std::string strErr{
        "Error missing path after import, found: " + tkStream_.current().raw() +
        " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (!std::filesystem::exists(fileName)) {
    const std::string strErr{
        "Error path " + fileName.string() +
        " not found from: " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (!parsedFiles_.count(fileName)) {
    tkStream_.insertAfter(lexer_.analyze(fileName));
  }
  return ASTBuilder::createImport(fileName, father);
}

std::shared_ptr<Node>
TopDown::parseVarDeclaration(std::shared_ptr<VariableTable> currentScope,
                             std::shared_ptr<Node> father) const {
  Token token{tkStream_.current()};
  if (token.type() == TokenType::LET) {
    tkStream_.eat();
    token = tkStream_.current();
    if (token.type() == TokenType::ID) {
      const std::string id{token.raw()};
      tkStream_.eat();
      if (tkStream_.current().type() == TokenType::DOTDOT) {
        tkStream_.eat();
      } else {
        const std::string strErr{"Error missing \':\' after " + id + " at " +
                                 tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
      token = tkStream_.current();
      if (token.type() == TokenType::ID) {
        const std::string idTypeStr{token.raw()};
        std::shared_ptr<GenericType> idType{typeTable_->type(idTypeStr)};
        tkStream_.eat();
        if (tkStream_.current().type() == TokenType::ASSIGNMENT) {
          tkStream_.eat();
          auto value{parseLogicalOr(currentScope, father)};
          return ASTBuilder::createVarDecl(id, idType, value, currentScope,
                                           typeTable_);
        } else {
          const std::string strErr{"Error missing value of " + id + " at " +
                                   tkStream_.current().locInfo()};
          llvm::report_fatal_error(strErr.c_str());
        }

      } else {
        const std::string strErr{"Error missing type of " + id + " at " +
                                 tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
    }
  } else if (token.type() == TokenType::CONST) {
    tkStream_.eat();
    token = tkStream_.current();
    if (token.type() == TokenType::ID) {
      const std::string id{token.raw()};
      tkStream_.eat();
      if (tkStream_.current().type() == TokenType::DOTDOT) {
        tkStream_.eat();
      } else {
        const std::string strErr{"Error missing \':\' after " + id + " at " +
                                 tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
      token = tkStream_.current();
      if (token.type() == TokenType::ID) {
        const std::string idTypeStr{token.raw()};
        std::shared_ptr<GenericType> idType{typeTable_->type(idTypeStr)};
        tkStream_.eat();
        if (tkStream_.current().type() == TokenType::ASSIGNMENT) {
          tkStream_.eat();
          auto value{parseLogicalOr(currentScope, father)};
          return ASTBuilder::createVarDecl(id, idType, value, currentScope,
                                           typeTable_);
        } else {
          const std::string strErr{"Error missing value of " + id + " at " +
                                   tkStream_.current().locInfo()};
          llvm::report_fatal_error(strErr.c_str());
        }

      } else {
        const std::string strErr{"Error missing type of " + id + " at " +
                                 tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
    }
  }

  return parseLogicalOr(currentScope, father);
}

std::shared_ptr<Node>
TopDown::parseLogicalOr(std::shared_ptr<VariableTable> currentScope,
                        std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseLogicalAnd(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         tkStream_.current().type() == TokenType::OR) {
    tkStream_.eat();
    auto right{parseLogicalAnd(currentScope, father)};
    left = ASTBuilder::createBinOp(left, TokenType::OR, right);
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseLogicalAnd(std::shared_ptr<VariableTable> currentScope,
                         std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseLogicalEqual(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         tkStream_.current().type() == TokenType::AND) {
    tkStream_.eat();
    auto right{parseLogicalEqual(currentScope, father)};
    left = ASTBuilder::createBinOp(left, TokenType::AND, right);
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseLogicalEqual(std::shared_ptr<VariableTable> currentScope,
                           std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseCompare(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current().type() == TokenType::EQUAL ||
          tkStream_.current().type() == TokenType::NOTEQUAL)) {
    const Token token{tkStream_.current()};
    tkStream_.eat();
    auto right{parseCompare(currentScope, father)};
    switch (token.type()) {
    case TokenType::EQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::EQUAL, right);
      break;
    case TokenType::NOTEQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::NOTEQUAL, right);
      break;
    default:
      const std::string strErr{"Cannot operate a binary expression with " +
                               tkStream_.current().raw() + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseCompare(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseAdd_Sub(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current().type() == TokenType::OPERATOR_SMALLER ||
          tkStream_.current().type() == TokenType::SMALLEREQUAL ||
          tkStream_.current().type() == TokenType::OPERATOR_GREATER ||
          tkStream_.current().type() == TokenType::BIGGEREQUAL)) {
    const Token token{tkStream_.current()};
    tkStream_.eat();
    auto right{parseAdd_Sub(currentScope, father)};
    switch (token.type()) {
    case TokenType::OPERATOR_SMALLER:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_SMALLER, right);
      break;
    case TokenType::SMALLEREQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::SMALLEREQUAL, right);
      break;
    case TokenType::OPERATOR_GREATER:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_GREATER, right);
      break;
    case TokenType::BIGGEREQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::BIGGEREQUAL, right);
      break;
    default:
      const std::string strErr{"Cannot operate a binary expression with " +
                               tkStream_.current().raw() + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseAdd_Sub(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseMult_Div(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current().type() == TokenType::OPERATOR_ADD ||
          tkStream_.current().type() == TokenType::OPERATOR_SUB)) {
    const Token token{tkStream_.current()};
    tkStream_.eat();
    auto right{parseMult_Div(currentScope, father)};
    switch (token.type()) {
    case TokenType::OPERATOR_ADD:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_ADD, right);
      break;
    case TokenType::OPERATOR_SUB:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_SUB, right);
      break;
    default:
      const std::string strErr{"Cannot operate a binary expression with " +
                               tkStream_.current().raw() + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseMult_Div(std::shared_ptr<VariableTable> currentScope,
                       std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseFactor(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current().type() == TokenType::OPERATOR_MULT ||
          tkStream_.current().type() == TokenType::OPERATOR_DIV ||
          tkStream_.current().type() == TokenType::OPERATOR_MODULE)) {
    const Token token{tkStream_.current()};
    tkStream_.eat();
    auto right{parseFactor(currentScope, father)};
    switch (token.type()) {
    case TokenType::OPERATOR_MULT:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_MULT, right);
      break;
    case TokenType::OPERATOR_DIV:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_DIV, right);
      break;
    case TokenType::OPERATOR_MODULE:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_MODULE, right);
      break;
    default:
      const std::string strErr{"Cannot operate a binary expression with " +
                               tkStream_.current().raw() + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseFactor(std::shared_ptr<VariableTable> currentScope,
                     std::shared_ptr<Node> father) const {
  switch (tkStream_.current().type()) {
  case TokenType::NUMBER_INT: {
    const int value{std::stoi(tkStream_.current().raw())};
    tkStream_.eat();
    return ASTBuilder::createInt(value, father);
  }
  case TokenType::NUMBER_DOUBLE: {
    const double value{std::stod(tkStream_.current().raw())};
    tkStream_.eat();
    return ASTBuilder::createDouble(value, father);
  }
  case TokenType::NUMBER_FLOAT: {
    const float value{std::stof(tkStream_.current().raw().substr(1))};
    tkStream_.eat();
    return ASTBuilder::createFloat(value, father);
  }
  case TokenType::STRING: {
    const std::string value{tkStream_.current().raw()};
    tkStream_.eat();
    return ASTBuilder::createString(value, father);
  }
  case TokenType::CHAR: {
    const std::string value{tkStream_.current().raw()};
    tkStream_.eat();
    return ASTBuilder::createChar(value, father);
  }
  case TokenType::TRUE: {
    tkStream_.eat();
    return ASTBuilder::createBool(true);
  }
  case TokenType::FALSE: {
    tkStream_.eat();
    return ASTBuilder::createBool(false);
  }
  case TokenType::OPERATOR_NOT: {
    tkStream_.eat();
    return ASTBuilder::createUnaryOp(TokenType::OPERATOR_NOT,
                                     parseLogicalOr(currentScope, father));
  }
  case TokenType::OPERATOR_SUB: {
    tkStream_.eat();
    return ASTBuilder::createUnaryOp(TokenType::OPERATOR_SUB,
                                     parseLogicalOr(currentScope, father));
  }
  case TokenType::INCREMENT: {
    tkStream_.eat();
    return ASTBuilder::createIncrement(TokenType::INCREMENT,
                                       parseLogicalOr(currentScope, father));
  }
  case TokenType::DECREMENT: {
    tkStream_.eat();
    return ASTBuilder::createIncrement(TokenType::DECREMENT,
                                       parseLogicalOr(currentScope, father));
  }
  case TokenType::ID: {
    const std::string id{tkStream_.current().raw()};
    tkStream_.eat();
    if (tkStream_.current().type() == TokenType::ASSIGNMENT) {
      tkStream_.eat();
      auto expression{parseLogicalOr(currentScope, father)};
      return ASTBuilder::createVarRGT(id, expression, currentScope, typeTable_);
    } else if (tkStream_.current().type() == TokenType::LB) {
      tkStream_.eat();
      auto attributes{parseComma(currentScope, father)};
      if (tkStream_.current().type() == TokenType::RB) {
        tkStream_.eat();
        return ASTBuilder::createStructConstr(id, attributes, typeTable_);
      }
    } else if (tkStream_.current().type() == TokenType::LP) {
      return parseFunctionCall(id, currentScope, father);
    }
    return ASTBuilder::createVarCall(id, currentScope);
  }
  case TokenType::STOP: {
    tkStream_.eat();
    return ASTBuilder::createStop(father);
  }
  case TokenType::PASS: {
    tkStream_.eat();
    return ASTBuilder::createPass(father);
  }
  case TokenType::LP: {
    tkStream_.eat();
    auto expression{parseLogicalOr(currentScope, father)};
    if (tkStream_.current().type() == TokenType::RP) {
      tkStream_.eat();
    } else {
      const std::string strErr{"Error: missing right parenthesis, found " +
                               tkStream_.current().raw() + " at" +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
    return expression;
  }
  default:
    const std::string strErr{"Error: unknown token found " +
                             tkStream_.current().raw() + " at" +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
}

std::shared_ptr<NodeFunctionCall>
TopDown::parseFunctionCall(const std::string &id,
                           std::shared_ptr<VariableTable> currentScope,
                           std::shared_ptr<Node> father) const {
  tkStream_.eat();
  std::vector<std::shared_ptr<Node>> params{};
  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
    return ASTBuilder::createFunctCall(id, params, currentScope,
                                       functionTable_);
  }
  params = parseParamsCall(currentScope, father);
  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing right ) for function " + id +
                             " at" + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  return ASTBuilder::createFunctCall(id, params, currentScope, functionTable_);
}

} // namespace nicole
