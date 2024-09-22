#include "../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include <cstdio>
#include <memory>

namespace nicole {

std::unique_ptr<Tree> TopDown::parse(const std::filesystem::path &path) const {
  tokens_ = lexer_.analyze(path);
  globalScope_ = std::make_shared<VariableTable>(nullptr);
  root_ = parseStart();
  return std::make_unique<Tree>(std::move(root_));
}

std::unique_ptr<NodeStatementList> TopDown::parseStart() const {
  std::vector<std::unique_ptr<NodeStatement>> gloablScopeStatements{};
  while (std::size_t(currentToken_) < tokens_.size()) {
    gloablScopeStatements.push_back(std::move(parseStatement(globalScope_)));
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::SEMICOLON)) {
      eat();
    }
  }

  return std::make_unique<NodeStatementList>(std::move(gloablScopeStatements));
}

std::unique_ptr<NodeStatementList>
TopDown::parseBody(std::shared_ptr<VariableTable> &bodyScope) const {
  if (getCurrentToken().type() == TokenType::LB) {
    eat();
  } else {
    const std::string strErr{"Error: missing left bracket, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  std::vector<std::unique_ptr<NodeStatement>> body;
  while (std::size_t(currentToken_) < tokens_.size() &&
         !isTokenType(TokenType::RB)) {
    body.push_back(std::move(parseStatement(bodyScope)));
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::SEMICOLON)) {
      eat();
    }
  }
  eat(); // Consume el token "}"
  return std::make_unique<NodeStatementList>(std::move(body));
}

std::unique_ptr<NodeStatement>
TopDown::parseStatement(std::shared_ptr<VariableTable> currentScope) const {
  switch (getCurrentToken().type()) {
  case TokenType::IF:
    return std::make_unique<NodeStatement>(parseIfStatement(currentScope));
  case TokenType::WHILE:
    return std::make_unique<NodeStatement>(parseWhileStatement(currentScope));
  case TokenType::PRINT:
    return std::make_unique<NodeStatement>(parsePrintStatement(currentScope));
  default:
    return std::make_unique<NodeStatement>(parseVarDeclaration(currentScope));
  }
}

std::unique_ptr<NodeIfStatement>
TopDown::parseIfStatement(std::shared_ptr<VariableTable> currentScope) const {
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

  auto condition{parseLogicalOr(currentScope)};

  if (getCurrentToken().type() == TokenType::RP) {
    eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto ifScope{std::make_shared<VariableTable>(currentScope)};
  auto ifBody{parseBody(ifScope)};
  std::unique_ptr<NodeStatementList> elseBody{nullptr};
  if (currentToken_ < tokens_.size() and
      getCurrentToken().type() == TokenType::ELSE) {
    eat();
    auto elseScope{std::make_shared<VariableTable>(currentScope)};
    elseBody = std::move(parseBody(elseScope));
  }
  return std::make_unique<NodeIfStatement>(
      std::move(condition), std::move(ifBody), std::move(elseBody));
}

std::unique_ptr<NodeWhileStatement> TopDown::parseWhileStatement(
    std::shared_ptr<VariableTable> currentScope) const {
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

  auto condition{parseLogicalOr(currentScope)};

  if (getCurrentToken().type() == TokenType::RP) {
    eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto whileScope{std::make_shared<VariableTable>(currentScope)};
  auto whileBody{parseBody(whileScope)};
  return std::make_unique<NodeWhileStatement>(std::move(condition),
                                              std::move(whileBody));
}

std::unique_ptr<NodePrint> TopDown::parsePrintStatement(
    std::shared_ptr<VariableTable> currentScope) const {
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
  auto expression{parseLogicalOr(currentScope)};
  if (getCurrentToken().type() == TokenType::RP) {
    eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             getCurrentToken().raw()};
    llvm::report_fatal_error(strErr.c_str());
  }
  return std::make_unique<NodePrint>(std::move(expression));
}

std::unique_ptr<Node> TopDown::parseVarDeclaration(
    std::shared_ptr<VariableTable> currentScope) const {
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
        std::unique_ptr<GenericType> idType{
            std::make_unique<GenericType>(idTypeStr)};
        eat();
        if (getCurrentToken().type() == TokenType::ASSIGNMENT) {
          eat();
          auto value{parseLogicalOr(currentScope)};
          return std::make_unique<NodeVariableDeclaration>(
              id, std::move(idType), std::move(value), currentScope);
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
        std::unique_ptr<GenericType> idType{
            std::make_unique<GenericType>(idTypeStr)};
        eat();
        if (getCurrentToken().type() == TokenType::ASSIGNMENT) {
          eat();
          auto value{parseLogicalOr(currentScope)};
          return std::make_unique<NodeConstDeclaration>(
              id, std::move(idType), std::move(value), currentScope);
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
  return parseLogicalOr(currentScope);
}

std::unique_ptr<Node>
TopDown::parseLogicalOr(std::shared_ptr<VariableTable> currentScope) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseLogicalAnd(currentScope)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         getCurrentToken().type() == TokenType::OR) {
    eat();
    auto right{parseLogicalAnd(currentScope)};
    left = std::make_unique<NodeBinaryOp>(std::move(left), TokenType::OR,
                                          std::move(right));
  }

  return left;
}

std::unique_ptr<Node>
TopDown::parseLogicalAnd(std::shared_ptr<VariableTable> currentScope) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseLogicalEqual(currentScope)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         getCurrentToken().type() == TokenType::AND) {
    eat();
    auto right{parseLogicalEqual(currentScope)};
    left = std::make_unique<NodeBinaryOp>(std::move(left), TokenType::AND,
                                          std::move(right));
  }

  return left;
}

std::unique_ptr<Node>
TopDown::parseLogicalEqual(std::shared_ptr<VariableTable> currentScope) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseCompare(currentScope)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::EQUAL ||
          getCurrentToken().type() == TokenType::NOTEQUAL)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseCompare(currentScope)};
    switch (token.type()) {
    case TokenType::EQUAL:
      left = std::make_unique<NodeBinaryOp>(std::move(left), TokenType::EQUAL,
                                            std::move(right));
      break;
    case TokenType::NOTEQUAL:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::NOTEQUAL, std::move(right));
      break;
    default:
      llvm::report_fatal_error("Error: invalid token type at parsing + or -");
    }
  }

  return left;
}

std::unique_ptr<Node>
TopDown::parseCompare(std::shared_ptr<VariableTable> currentScope) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseAdd_Sub(currentScope)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::OPERATOR_SMALLER ||
          getCurrentToken().type() == TokenType::SMALLEREQUAL ||
          getCurrentToken().type() == TokenType::OPERATOR_GREATER ||
          getCurrentToken().type() == TokenType::BIGGEREQUAL)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseAdd_Sub(currentScope)};
    switch (token.type()) {
    case TokenType::OPERATOR_SMALLER:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::OPERATOR_SMALLER, std::move(right));
      break;
    case TokenType::SMALLEREQUAL:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::SMALLEREQUAL, std::move(right));
      break;
    case TokenType::OPERATOR_GREATER:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::OPERATOR_GREATER, std::move(right));
      break;
    case TokenType::BIGGEREQUAL:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::BIGGEREQUAL, std::move(right));
      break;
    default:
      llvm::report_fatal_error("Error: invalid token type at parsing + or -");
    }
  }

  return left;
}

std::unique_ptr<Node>
TopDown::parseAdd_Sub(std::shared_ptr<VariableTable> currentScope) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseMult_Div(currentScope)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::OPERATOR_ADD ||
          getCurrentToken().type() == TokenType::OPERATOR_SUB)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseMult_Div(currentScope)};
    switch (token.type()) {
    case TokenType::OPERATOR_ADD:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::OPERATOR_ADD, std::move(right));
      break;
    case TokenType::OPERATOR_SUB:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::OPERATOR_SUB, std::move(right));
      break;
    default:
      llvm::report_fatal_error("Error: invalid token type at parsing + or -");
    }
  }

  return left;
}

std::unique_ptr<Node>
TopDown::parseMult_Div(std::shared_ptr<VariableTable> currentScope) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseFactor(currentScope)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::OPERATOR_MULT ||
          getCurrentToken().type() == TokenType::OPERATOR_DIV ||
          getCurrentToken().type() == TokenType::OPERATOR_MODULE)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseFactor(currentScope)};
    switch (token.type()) {
    case TokenType::OPERATOR_MULT:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::OPERATOR_MULT, std::move(right));
      break;
    case TokenType::OPERATOR_DIV:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::OPERATOR_DIV, std::move(right));
      break;
    case TokenType::OPERATOR_MODULE:
      left = std::make_unique<NodeBinaryOp>(
          std::move(left), TokenType::OPERATOR_MODULE, std::move(right));
      break;
    default:
      llvm::report_fatal_error("Error: invalid token type at parsing + or -");
    }
  }

  return left;
}

std::unique_ptr<Node>
TopDown::parseFactor(std::shared_ptr<VariableTable> currentScope) const {
  switch (getCurrentToken().type()) {
  case TokenType::NUMBER_INT: {
    const int value{std::stoi(getCurrentToken().raw())};
    eat();
    return std::make_unique<NodeLiteralInt>(value);
  }
  case TokenType::NUMBER_DOUBLE: {
    const double value{std::stod(getCurrentToken().raw())};
    eat();
    return std::make_unique<NodeLiteralDouble>(value);
  }
  case TokenType::STRING: {
    const std::string value{getCurrentToken().raw()};
    eat();
    return std::make_unique<NodeLiteralString>(value);
  }
  case TokenType::CHAR: {
    const char value{getCurrentToken().raw()[0]};
    eat();
    return std::make_unique<NodeLiteralChar>(value);
  }
  case TokenType::TRUE: {
    eat();
    return std::make_unique<NodeLiteralBool>(true);
  }
  case TokenType::FALSE: {
    eat();
    return std::make_unique<NodeLiteralBool>(false);
  }
  case TokenType::OPERATOR_NOT: {
    eat();
    return std::make_unique<NodeUnaryOp>(TokenType::OPERATOR_NOT,
                                         parseLogicalOr(currentScope));
  }
  case TokenType::OPERATOR_SUB: {
    eat();
    return std::make_unique<NodeUnaryOp>(TokenType::OPERATOR_SUB,
                                         parseLogicalOr(currentScope));
  }
  case TokenType::INCREMENT: {
    eat();
    return std::make_unique<NodeIncrement>(TokenType::INCREMENT,
                                           parseLogicalOr(currentScope));
  }
  case TokenType::DECREMENT: {
    eat();
    return std::make_unique<NodeIncrement>(TokenType::DECREMENT,
                                           parseLogicalOr(currentScope));
  }
  case TokenType::ID: {
    const std::string id{getCurrentToken().raw()};
    eat();
    if (getCurrentToken().type() == TokenType::ASSIGNMENT) {
      eat();
      auto expression{parseLogicalOr(currentScope)};
      return std::make_unique<NodeVariableReassignment>(
          id, std::move(expression), currentScope);
    }
    return std::make_unique<NodeVariableCall>(id, currentScope);
  }
  case TokenType::LP: {
    eat();
    auto expression{parseLogicalOr(currentScope)};
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
