#include "../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"

namespace nicole {

std::unique_ptr<Tree> TopDown::parse(const std::filesystem::path& path) const {
  tokens_ = lexer_.analyze(path);
  globalScope_ = std::make_shared<VariableTable>(nullptr);
  root_ = parseStart();
  std::cout << *globalScope_;
  return std::make_unique<Tree>(std::move(root_));
}

std::unique_ptr<NodeStatementList> TopDown::parseStart() const {
  std::vector<std::unique_ptr<NodeStatement>> gloablScopeStatements;
  while (std::size_t(currentToken_) < tokens_.size()) {
    gloablScopeStatements.push_back(std::move(parseStatement(globalScope_)));
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::SEMICOLON)) {
      eat();
    }
  }

  return std::make_unique<NodeStatementList>(std::move(gloablScopeStatements));
}

std::unique_ptr<NodeStatement> TopDown::parseStatement(std::shared_ptr<VariableTable> currentScope) const {
  return std::make_unique<NodeStatement>(parseVarDeclaration(currentScope));
}

std::unique_ptr<Node> TopDown::parseVarDeclaration(std::shared_ptr<VariableTable> currentScope) const {
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
          auto value{parseAdd_Sub(currentScope)};
          if (!currentScope) llvm::report_fatal_error("ADIOS");
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
  }
  return parseAdd_Sub(currentScope);
}

std::unique_ptr<Node> TopDown::parseAdd_Sub(std::shared_ptr<VariableTable> currentScope) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseFactor(currentScope)};

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::OPERATOR_ADD ||
          getCurrentToken().type() == TokenType::OPERATOR_SUB)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseFactor(currentScope)};
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

std::unique_ptr<Node> TopDown::parseFactor(std::shared_ptr<VariableTable> currentScope) const {
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
    case TokenType::LP: {
      eat();
      auto expression{parseAdd_Sub(currentScope)};
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

}  // namespace nicole
