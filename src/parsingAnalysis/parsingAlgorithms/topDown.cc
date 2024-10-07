#include "../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <memory>

namespace nicole {

std::shared_ptr<Tree> TopDown::parse(const std::filesystem::path &path) const {
  tokens_ = lexer_.analyze(path, false);
  globalScope_ = std::make_shared<VariableTable>(nullptr);
  typeTable_ = std::make_shared<TypeTable>();
  functionTable_ = std::make_shared<FunctionTable>();
  root_ = parseStart();

  return std::make_shared<Tree>(root_);
}

std::shared_ptr<NodeStatementList> TopDown::parseStart() const {
  std::vector<std::shared_ptr<NodeStatement>> gloablScopeStatements{};
  std::shared_ptr<Node> father{nullptr};
  while (std::size_t(currentToken_) < tokens_.size()) {
    gloablScopeStatements.push_back(parseStatement(globalScope_, father));
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::SEMICOLON)) {
      eat();
    }
  }

  return ASTBuilder::createList(gloablScopeStatements);
}

std::shared_ptr<NodeStatementList>
TopDown::parseBody(std::shared_ptr<VariableTable> &bodyScope,
                   std::shared_ptr<Node> father) const {
  if (getCurrentToken().type() == TokenType::LB) {
    eat();
  } else {
    const std::string strErr{"Error: missing left bracket, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  std::vector<std::shared_ptr<NodeStatement>> body;
  while (std::size_t(currentToken_) < tokens_.size() &&
         !isTokenType(TokenType::RB)) {
    body.push_back(parseStatement(bodyScope, father));
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::SEMICOLON)) {
      eat();
    }
  }
  eat(); // Consume el token "}"
  return ASTBuilder::createList(body);
}

std::shared_ptr<NodeStatementList>
TopDown::parseComma(std::shared_ptr<VariableTable> &currentScope,
                    std::shared_ptr<Node> father) const {
  std::vector<std::shared_ptr<NodeStatement>> body;
  while (std::size_t(currentToken_) < tokens_.size() &&
         !isTokenType(TokenType::SEMICOLON)) {
    body.push_back(ASTBuilder::createStatement(
        parseVarDeclaration(currentScope, father)));
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::COMMA)) {
      eat();
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
  if (getCurrentToken().type() == TokenType::RP) {
    return std::make_shared<ParamsDeclaration>(params, currentScope);
  }
  while (std::size_t(currentToken_) < tokens_.size() &&
         !isTokenType(TokenType::SEMICOLON)) {
    auto token{getCurrentToken()};
    std::string id{};
    std::shared_ptr<GenericType> type{nullptr};
    if (token.type() == TokenType::ID) {
      id = token.raw();
      eat();
    } else {
      llvm::report_fatal_error("Expected parameter, found: ,");
    }
    if (getCurrentToken().type() == TokenType::DOTDOT) {
      eat();
    } else {
      llvm::report_fatal_error("Expected :, found: ");
    }
    token = getCurrentToken();
    if (token.type() == TokenType::ID) {
      type = typeTable_->type(token.raw());
      eat();
    } else {
      llvm::report_fatal_error("Expected type, found: ");
    }
    params.push_back({id, type});
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::COMMA)) {
      eat();
    } else {
      break;
    }
  }
  return std::make_shared<ParamsDeclaration>(params, currentScope);
}

std::shared_ptr<NodeStatement>
TopDown::parseStatement(std::shared_ptr<VariableTable> currentScope,
                        std::shared_ptr<Node> father) const {
  switch (getCurrentToken().type()) {
  case TokenType::IF: {
    return ASTBuilder::createStatement(
        parseIfStatement(currentScope, father), father);
  }
  case TokenType::WHILE: {
    return ASTBuilder::createStatement(
        parseWhileStatement(currentScope, father), father);
  }
  case TokenType::FOR: {
    return ASTBuilder::createStatement(
        parseForStatement(currentScope, father), father);
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
  default: {
    return ASTBuilder::createStatement(
        parseVarDeclaration(currentScope, father), father);
  }
  }
}

std::shared_ptr<NodeIfStatement>
TopDown::parseIfStatement(std::shared_ptr<VariableTable> currentScope,
                          std::shared_ptr<Node> father) const {
  eat();
  if (getCurrentToken().type() == TokenType::LP) {
    eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (getCurrentToken().type() == TokenType::RP) {
    const std::string strErr{"Error: empty if condition, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto condition{parseLogicalOr(currentScope, father)};

  if (getCurrentToken().type() == TokenType::RP) {
    eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto ifScope{std::make_shared<VariableTable>(currentScope)};
  auto ifBody{parseBody(ifScope, father)};
  std::shared_ptr<NodeStatementList> elseBody{nullptr};
  if (currentToken_ < tokens_.size() and
      getCurrentToken().type() == TokenType::ELSE) {
    eat();
    auto elseScope{std::make_shared<VariableTable>(currentScope)};
    elseBody = parseBody(elseScope, father);
  }
  return ASTBuilder::createIf(condition, ifBody, elseBody);
}

std::shared_ptr<NodeWhileStatement>
TopDown::parseWhileStatement(std::shared_ptr<VariableTable> currentScope,
                             std::shared_ptr<Node> father) const {
  eat();
  if (getCurrentToken().type() == TokenType::LP) {
    eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (getCurrentToken().type() == TokenType::RP) {
    const std::string strErr{"Error: empty while condition, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto condition{parseLogicalOr(currentScope, father)};

  if (getCurrentToken().type() == TokenType::RP) {
    eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto whileScope{std::make_shared<VariableTable>(currentScope)};
  auto whileBody{parseBody(whileScope, father)};
  return ASTBuilder::createWhile(condition, whileBody);
}

std::shared_ptr<NodeForStatement>
TopDown::parseForStatement(std::shared_ptr<VariableTable> currentScope,
                           std::shared_ptr<Node> father) const {
  eat();
  if (getCurrentToken().type() == TokenType::LP) {
    eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (getCurrentToken().type() == TokenType::RP) {
    const std::string strErr{"Error: empty for, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto forScope{std::make_shared<VariableTable>(currentScope)};
  auto init{parseComma(forScope, father)};
  if (getCurrentToken().type() == TokenType::SEMICOLON) {
    eat();
  } else {
    const std::string strErr{"Error: missing \";\" to separate init from "
                             "condition in for statement, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto condition{parseLogicalOr(forScope, father)};
  if (getCurrentToken().type() == TokenType::SEMICOLON) {
    eat();
  } else {
    const std::string strErr{"Error: missing \";\" to separate condition from "
                             "update in for statement, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto update{parseComma(forScope, father)};
  if (getCurrentToken().type() == TokenType::RP) {
    eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto body{parseBody(forScope, father)};
  return ASTBuilder::createFor(init, condition, update, body);
}

std::shared_ptr<NodePrint>
TopDown::parsePrintStatement(std::shared_ptr<VariableTable> currentScope,
                             std::shared_ptr<Node> father) const {
  eat();
  if (getCurrentToken().type() == TokenType::LP) {
    eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (getCurrentToken().type() == TokenType::RP) {
    const std::string strErr{"Error: empty print, found "};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto expression{parseLogicalOr(currentScope, father)};
  if (getCurrentToken().type() == TokenType::RP) {
    eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  return ASTBuilder::createPrint(expression);
}

std::shared_ptr<NodeStructDeclaration>
TopDown::parseStructDeclaration(std::shared_ptr<VariableTable> currentScope,
                                std::shared_ptr<Node> father) const {
  eat();
  auto token{getCurrentToken()};
  std::shared_ptr<NodeStatementList> body{nullptr};
  std::shared_ptr<GenericType> idType{nullptr};
  if (token.type() == TokenType::ID) {
    const std::string idTypeStr{token.raw()};
    idType = std::make_shared<UserType>(idTypeStr);
    typeTable_->addType(idType);
    eat();
    if (getCurrentToken().type() == TokenType::LB) {
      auto structScope{std::make_shared<VariableTable>(nullptr)};
      body = parseBody(structScope, father);
    } else {
      const std::string strErr{"Error missing left { of " + idTypeStr};
      llvm::report_fatal_error(strErr.c_str());
    }
  }
  return ASTBuilder::createStructDecl(idType, body);
}

std::shared_ptr<NodeFunctionDeclaration>
TopDown::parseFunctionDeclaration(std::shared_ptr<VariableTable> currentScope,
                                  std::shared_ptr<Node> father) const {
  eat();
  auto token{getCurrentToken()};
  std::string id{};
  if (token.type() == TokenType::ID) {
    id = token.raw();
    eat();
  } else {
    const std::string strErr{"Error missing function id after def"};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (getCurrentToken().type() == TokenType::LP) {
    eat();
  } else {
    const std::string strErr{"Error missing left ( for function " + id};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto funScope{std::make_shared<VariableTable>(nullptr)};
  std::shared_ptr<ParamsDeclaration> params{parseParams(funScope, father)};
  if (getCurrentToken().type() == TokenType::RP) {
    eat();
  } else {
    const std::string strErr{"Error missing right ) for function " + id};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (getCurrentToken().type() == TokenType::DOTDOT) {
    eat();
  } else {
    const std::string strErr{"Error missing \':\' after right ) for function " +
                             id};
    llvm::report_fatal_error(strErr.c_str());
  }
  token = getCurrentToken();
  std::shared_ptr<GenericType> returnType{nullptr};
  if (token.type() == TokenType::ID) {
    returnType = typeTable_->type(token.raw());
    eat();
  } else {
    const std::string strErr{"Error missing return type for the function " +
                             id};
    llvm::report_fatal_error(strErr.c_str());
  }
  std::shared_ptr<NodeStatementList> body{parseBody(funScope, father)};
  return ASTBuilder::createFunctDecl(id, params, returnType, body,
                                                   funScope, typeTable_, functionTable_);
}

std::shared_ptr<NodeReturn>
TopDown::parseReturn(std::shared_ptr<VariableTable> &currentScope,
                     std::shared_ptr<Node> father) const {
  eat();
  if (isTokenType(TokenType::SEMICOLON)) {
    return ASTBuilder::createReturn(nullptr);
  }
  return ASTBuilder::createReturn(parseLogicalOr(currentScope, father));
}

std::shared_ptr<Node>
TopDown::parseVarDeclaration(std::shared_ptr<VariableTable> currentScope,
                             std::shared_ptr<Node> father) const {
  Token token{getCurrentToken()};
  if (token.type() == TokenType::LET) {
    eat();
    token = getCurrentToken();
    if (token.type() == TokenType::ID) {
      const std::string id{token.raw()};
      eat();
      if (getCurrentToken().type() == TokenType::DOTDOT) {
        eat();
      } else {
        const std::string strErr{"Error missing \':\' after " + id};
        llvm::report_fatal_error(strErr.c_str());
      }
      token = getCurrentToken();
      if (token.type() == TokenType::ID) {
        const std::string idTypeStr{token.raw()};
        std::shared_ptr<GenericType> idType{typeTable_->type(idTypeStr)};
        eat();
        if (getCurrentToken().type() == TokenType::ASSIGNMENT) {
          eat();
          auto value{parseLogicalOr(currentScope, father)};
          return ASTBuilder::createVarDecl(
              id, idType, value, currentScope, typeTable_);
        } else {
          const std::string strErr{"Error missing value of " + id};
          llvm::report_fatal_error(strErr.c_str());
        }

      } else {
        const std::string strErr{"Error missing type of " + id};
        llvm::report_fatal_error(strErr.c_str());
      }
    }
  } else if (token.type() == TokenType::CONST) {
    eat();
    token = getCurrentToken();
    if (token.type() == TokenType::ID) {
      const std::string id{token.raw()};
      eat();
      if (getCurrentToken().type() == TokenType::DOTDOT) {
        eat();
      } else {
        const std::string strErr{"Error missing \':\' after " + id};
        llvm::report_fatal_error(strErr.c_str());
      }
      token = getCurrentToken();
      if (token.type() == TokenType::ID) {
        const std::string idTypeStr{token.raw()};
        std::shared_ptr<GenericType> idType{typeTable_->type(idTypeStr)};
        eat();
        if (getCurrentToken().type() == TokenType::ASSIGNMENT) {
          eat();
          auto value{parseLogicalOr(currentScope, father)};
          return ASTBuilder::createVarDecl(
              id, idType, value, currentScope, typeTable_);
        } else {
          const std::string strErr{"Error missing value of " + id};
          llvm::report_fatal_error(strErr.c_str());
        }

      } else {
        const std::string strErr{"Error missing type of " + id};
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

  while (std::size_t(currentToken_) < tokens_.size() &&
         getCurrentToken().type() == TokenType::OR) {
    eat();
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

  while (std::size_t(currentToken_) < tokens_.size() &&
         getCurrentToken().type() == TokenType::AND) {
    eat();
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

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::EQUAL ||
          getCurrentToken().type() == TokenType::NOTEQUAL)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseCompare(currentScope, father)};
    switch (token.type()) {
    case TokenType::EQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::EQUAL, right);
      break;
    case TokenType::NOTEQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::NOTEQUAL, right);
      break;
    default:
      llvm::report_fatal_error("Error: invalid token type at parsing + or -");
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseCompare(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseAdd_Sub(currentScope, father)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::OPERATOR_SMALLER ||
          getCurrentToken().type() == TokenType::SMALLEREQUAL ||
          getCurrentToken().type() == TokenType::OPERATOR_GREATER ||
          getCurrentToken().type() == TokenType::BIGGEREQUAL)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseAdd_Sub(currentScope, father)};
    switch (token.type()) {
    case TokenType::OPERATOR_SMALLER:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_SMALLER,
                                            right);
      break;
    case TokenType::SMALLEREQUAL:
      left =
          ASTBuilder::createBinOp(left, TokenType::SMALLEREQUAL, right);
      break;
    case TokenType::OPERATOR_GREATER:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_GREATER,
                                            right);
      break;
    case TokenType::BIGGEREQUAL:
      left =
          ASTBuilder::createBinOp(left, TokenType::BIGGEREQUAL, right);
      break;
    default:
      llvm::report_fatal_error("Error: invalid token type at parsing + or -");
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseAdd_Sub(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseMult_Div(currentScope, father)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::OPERATOR_ADD ||
          getCurrentToken().type() == TokenType::OPERATOR_SUB)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseMult_Div(currentScope, father)};
    switch (token.type()) {
    case TokenType::OPERATOR_ADD:
      left =
          ASTBuilder::createBinOp(left, TokenType::OPERATOR_ADD, right);
      break;
    case TokenType::OPERATOR_SUB:
      left =
          ASTBuilder::createBinOp(left, TokenType::OPERATOR_SUB, right);
      break;
    default:
      llvm::report_fatal_error("Error: invalid token type at parsing + or -");
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseMult_Div(std::shared_ptr<VariableTable> currentScope,
                       std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseFactor(currentScope, father)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::OPERATOR_MULT ||
          getCurrentToken().type() == TokenType::OPERATOR_DIV ||
          getCurrentToken().type() == TokenType::OPERATOR_MODULE)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseFactor(currentScope, father)};
    switch (token.type()) {
    case TokenType::OPERATOR_MULT:
      left =
          ASTBuilder::createBinOp(left, TokenType::OPERATOR_MULT, right);
      break;
    case TokenType::OPERATOR_DIV:
      left =
          ASTBuilder::createBinOp(left, TokenType::OPERATOR_DIV, right);
      break;
    case TokenType::OPERATOR_MODULE:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_MODULE,
                                            right);
      break;
    default:
      llvm::report_fatal_error("Error: invalid token type at parsing + or -");
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseFactor(std::shared_ptr<VariableTable> currentScope,
                     std::shared_ptr<Node> father) const {
  switch (getCurrentToken().type()) {
  case TokenType::NUMBER_INT: {
    const int value{std::stoi(getCurrentToken().raw())};
    eat();
    return ASTBuilder::createInt(value, father);
  }
  case TokenType::NUMBER_DOUBLE: {
    const double value{std::stod(getCurrentToken().raw())};
    eat();
    return ASTBuilder::createDouble(value, father);
  }
  case TokenType::NUMBER_FLOAT: {
    const float value{std::stof(getCurrentToken().raw().substr(1))};
    eat();
    return ASTBuilder::createFloat(value, father);
  }
  case TokenType::STRING: {
    const std::string value{getCurrentToken().raw()};
    eat();
    return ASTBuilder::createString(value, father);
  }
  case TokenType::CHAR: {
    const std::string value{getCurrentToken().raw()};
    eat();
    return ASTBuilder::createChar(value, father);
  }
  case TokenType::TRUE: {
    eat();
    return ASTBuilder::createBool(true);
  }
  case TokenType::FALSE: {
    eat();
    return ASTBuilder::createBool(false);
  }
  case TokenType::OPERATOR_NOT: {
    eat();
    return ASTBuilder::createUnaryOp(TokenType::OPERATOR_NOT,
                                         parseLogicalOr(currentScope, father));
  }
  case TokenType::OPERATOR_SUB: {
    eat();
    return ASTBuilder::createUnaryOp(TokenType::OPERATOR_SUB,
                                         parseLogicalOr(currentScope, father));
  }
  case TokenType::INCREMENT: {
    eat();
    return ASTBuilder::createIncrement(
        TokenType::INCREMENT, parseLogicalOr(currentScope, father));
  }
  case TokenType::DECREMENT: {
    eat();
    return ASTBuilder::createIncrement(
        TokenType::DECREMENT, parseLogicalOr(currentScope, father));
  }
  case TokenType::ID: {
    const std::string id{getCurrentToken().raw()};
    eat();
    if (getCurrentToken().type() == TokenType::ASSIGNMENT) {
      eat();
      auto expression{parseLogicalOr(currentScope, father)};
      return ASTBuilder::createVarRGT(
          id, expression, currentScope, typeTable_);
    } else if (getCurrentToken().type() == TokenType::LB) {
      eat();
      auto attributes{parseComma(currentScope, father)};
      if (getCurrentToken().type() == TokenType::RB) {
        eat();
        return ASTBuilder::createStructConstr(id, attributes,
                                                       typeTable_);
      }
    } else if (getCurrentToken().type() == TokenType::LP) {
      return parseFunctionCall(id, currentScope, father);
    }
    return ASTBuilder::createVarCall(id, currentScope);
  }
  case TokenType::STOP: {
    eat();
    return ASTBuilder::createStop(father);
  }
  case TokenType::PASS: {
    eat();
    return ASTBuilder::createPass(father);
  }
  case TokenType::LP: {
    eat();
    auto expression{parseLogicalOr(currentScope, father)};
    if (getCurrentToken().type() == TokenType::RP) {
      eat();
    } else {
      const std::string strErr{"Error: missing right parenthesis, found " +
                               getCurrentToken().raw()};
      llvm::report_fatal_error(strErr.c_str());
    }
    return expression;
  }
  default:
    const std::string strErr{"Error: unknown token found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
}

std::shared_ptr<NodeFunctionCall>
TopDown::parseFunctionCall(const std::string& id, std::shared_ptr<VariableTable> currentScope,
                           std::shared_ptr<Node> father) const {
  eat();
  std::vector<std::shared_ptr<Node>> params{};
  if (getCurrentToken().type() == TokenType::RP) {
    eat();
    return ASTBuilder::createFunctCall(id, params, currentScope, functionTable_);
  }
  while (std::size_t(currentToken_) < tokens_.size() &&
         !isTokenType(TokenType::SEMICOLON)) {
    params.push_back(parseLogicalOr(currentScope, father));
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::COMMA)) {
      eat();
    } else {
      break;
    }
  }
  if (getCurrentToken().type() == TokenType::RP) {
    eat();
  } else {
    const std::string strErr{"Error missing right ) for function " + id};
    llvm::report_fatal_error(strErr.c_str());
  }
  return ASTBuilder::createFunctCall(id, params, currentScope, functionTable_);
}

} // namespace nicole
