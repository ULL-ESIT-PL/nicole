#include "../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <memory>

namespace nicole {

std::shared_ptr<Tree> TopDown::parse(const std::filesystem::path &path) const {
  tokens_ = lexer_.analyze(path);
  globalScope_ = std::make_shared<VariableTable>(nullptr);
  typeTable_ = std::make_shared<TypeTable>();
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
  return std::make_shared<NodeStatementList>(gloablScopeStatements);
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
  return std::make_shared<NodeStatementList>(body);
}

std::shared_ptr<NodeStatementList>
TopDown::parseComma(std::shared_ptr<VariableTable> &currentScope,
                    std::shared_ptr<Node> father) const {
  std::vector<std::shared_ptr<NodeStatement>> body;
  while (std::size_t(currentToken_) < tokens_.size() &&
         !isTokenType(TokenType::SEMICOLON)) {
    body.push_back(std::make_shared<NodeStatement>(
        parseVarDeclaration(currentScope, father)));
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::COMMA)) {
      eat();
    } else {
      break;
    }
  }
  return std::make_shared<NodeStatementList>(body);
}

std::shared_ptr<ParamsDeclaration>
TopDown::parseParams(std::shared_ptr<VariableTable> &currentScope,
                     std::shared_ptr<Node> father) const {
  std::vector<std::pair<std::string, std::shared_ptr<GenericType>>> params;
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
    return std::make_shared<NodeStatement>(
        parseIfStatement(currentScope, father), father);
  }
  case TokenType::WHILE: {
    return std::make_shared<NodeStatement>(
        parseWhileStatement(currentScope, father), father);
  }
  case TokenType::FOR: {
    return std::make_shared<NodeStatement>(
        parseForStatement(currentScope, father), father);
  }
  case TokenType::PRINT: {
    return std::make_shared<NodeStatement>(
        parsePrintStatement(currentScope, father), father);
  }
  case TokenType::STRUCT: {
    return std::make_shared<NodeStatement>(
        parseStructDeclaration(currentScope, father), father);
  }
  case TokenType::FUNCTION: {
    return std::make_shared<NodeStatement>(
        parseFunctionDeclaration(currentScope, father), father);
  }
  default: {
    return std::make_shared<NodeStatement>(
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
  return std::make_shared<NodeIfStatement>(condition, ifBody, elseBody);
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
  return std::make_shared<NodeWhileStatement>(condition, whileBody);
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
  return std::make_shared<NodeForStatement>(init, condition, update, body);
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
  return std::make_shared<NodePrint>(expression);
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
  return std::make_shared<NodeStructDeclaration>(idType, body);
}

std::shared_ptr<NodeFunctionDeclaration>
TopDown::parseFunctionDeclaration(std::shared_ptr<VariableTable> currentScope,
                                  std::shared_ptr<Node> father) const {
  eat();
  auto token{getCurrentToken()};
  std::string id{};
  std::shared_ptr<ParamsDeclaration> params{nullptr};
  std::shared_ptr<GenericType> returnType{nullptr};
  std::shared_ptr<ParamsDeclaration> body{nullptr};
  return nullptr;
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
          return std::make_shared<NodeVariableDeclaration>(
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
          return std::make_shared<NodeVariableDeclaration>(
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
    left = std::make_shared<NodeBinaryOp>(left, TokenType::OR, right);
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
    left = std::make_shared<NodeBinaryOp>(left, TokenType::AND, right);
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
      left = std::make_shared<NodeBinaryOp>(left, TokenType::EQUAL, right);
      break;
    case TokenType::NOTEQUAL:
      left = std::make_shared<NodeBinaryOp>(left, TokenType::NOTEQUAL, right);
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
      left = std::make_shared<NodeBinaryOp>(left, TokenType::OPERATOR_SMALLER,
                                            right);
      break;
    case TokenType::SMALLEREQUAL:
      left =
          std::make_shared<NodeBinaryOp>(left, TokenType::SMALLEREQUAL, right);
      break;
    case TokenType::OPERATOR_GREATER:
      left = std::make_shared<NodeBinaryOp>(left, TokenType::OPERATOR_GREATER,
                                            right);
      break;
    case TokenType::BIGGEREQUAL:
      left =
          std::make_shared<NodeBinaryOp>(left, TokenType::BIGGEREQUAL, right);
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
          std::make_shared<NodeBinaryOp>(left, TokenType::OPERATOR_ADD, right);
      break;
    case TokenType::OPERATOR_SUB:
      left =
          std::make_shared<NodeBinaryOp>(left, TokenType::OPERATOR_SUB, right);
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
          std::make_shared<NodeBinaryOp>(left, TokenType::OPERATOR_MULT, right);
      break;
    case TokenType::OPERATOR_DIV:
      left =
          std::make_shared<NodeBinaryOp>(left, TokenType::OPERATOR_DIV, right);
      break;
    case TokenType::OPERATOR_MODULE:
      left = std::make_shared<NodeBinaryOp>(left, TokenType::OPERATOR_MODULE,
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
    return std::make_shared<NodeLiteralInt>(value);
  }
  case TokenType::NUMBER_DOUBLE: {
    const double value{std::stod(getCurrentToken().raw())};
    eat();
    return std::make_shared<NodeLiteralDouble>(value);
  }
  case TokenType::STRING: {
    const std::string value{getCurrentToken().raw()};
    eat();
    return std::make_shared<NodeLiteralString>(value);
  }
  case TokenType::CHAR: {
    const char value{getCurrentToken().raw()[1]};
    eat();
    return std::make_shared<NodeLiteralChar>(value);
  }
  case TokenType::TRUE: {
    eat();
    return std::make_shared<NodeLiteralBool>(true);
  }
  case TokenType::FALSE: {
    eat();
    return std::make_shared<NodeLiteralBool>(false);
  }
  case TokenType::OPERATOR_NOT: {
    eat();
    return std::make_shared<NodeUnaryOp>(TokenType::OPERATOR_NOT,
                                         parseLogicalOr(currentScope, father));
  }
  case TokenType::OPERATOR_SUB: {
    eat();
    return std::make_shared<NodeUnaryOp>(TokenType::OPERATOR_SUB,
                                         parseLogicalOr(currentScope, father));
  }
  case TokenType::INCREMENT: {
    eat();
    return std::make_shared<NodeIncrement>(
        TokenType::INCREMENT, parseLogicalOr(currentScope, father));
  }
  case TokenType::DECREMENT: {
    eat();
    return std::make_shared<NodeIncrement>(
        TokenType::DECREMENT, parseLogicalOr(currentScope, father));
  }
  case TokenType::ID: {
    const std::string id{getCurrentToken().raw()};
    eat();
    if (getCurrentToken().type() == TokenType::ASSIGNMENT) {
      eat();
      auto expression{parseLogicalOr(currentScope, father)};
      return std::make_shared<NodeVariableReassignment>(
          id, expression, currentScope, typeTable_);
    } else if (getCurrentToken().type() == TokenType::LB) {
      eat();
      auto attributes{parseComma(currentScope, father)};
      if (getCurrentToken().type() == TokenType::RB) {
        eat();
        return std::make_shared<NodeStructConstructor>(id, attributes,
                                                       typeTable_);
      }
    }
    return std::make_shared<NodeVariableCall>(id, currentScope);
  }
  case TokenType::STOP: {
    eat();
    return std::make_shared<NodeStop>(father);
  }
  case TokenType::PASS: {
    eat();
    return std::make_shared<NodePass>(father);
  }
  case TokenType::RETURN: {
    eat();
    if (getCurrentToken().type() == TokenType::SEMICOLON) {
      return std::make_shared<NodeReturn>(nullptr);
    }
    return std::make_shared<NodeReturn>(parseLogicalOr(currentScope, father));
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

} // namespace nicole
