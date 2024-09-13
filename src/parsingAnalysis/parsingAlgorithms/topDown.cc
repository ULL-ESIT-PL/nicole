#include "../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"

namespace nicole {
std::unique_ptr<Tree> TopDown::parse(const std::filesystem::path& path) const {
  tokens_ = lexer_.analyze(path);
  root_ = parseStart();
  return std::make_unique<Tree>(std::move(root_));
}

std::unique_ptr<NodeStatementList> TopDown::parseStart() const {
  std::vector<std::unique_ptr<NodeStatement>> gloablScopeStatements;
  while (std::size_t(currentToken_) < tokens_.size()) {
    gloablScopeStatements.push_back(std::move(parseStatement()));
    if (std::size_t(currentToken_) < tokens_.size() &&
        isTokenType(TokenType::SEMICOLON)) {
      eat();
    }
  }

  return std::make_unique<NodeStatementList>(std::move(gloablScopeStatements));
}

std::unique_ptr<NodeStatement> TopDown::parseStatement() const {
  return std::make_unique<NodeStatement>(parseAdd_Sub());
}

std::unique_ptr<Node> TopDown::parseAdd_Sub() const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseFactor()};

  while (std::size_t(currentToken_) < tokens_.size() &&
         (getCurrentToken().type() == TokenType::OPERATOR_ADD ||
          getCurrentToken().type() == TokenType::OPERATOR_SUB)) {
    const Token token{getCurrentToken()};
    eat();
    auto right{parseFactor()};
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

std::unique_ptr<Node> TopDown::parseFactor() const {
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
    case TokenType::LP: {
      eat();
      auto expression{parseAdd_Sub()};
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
