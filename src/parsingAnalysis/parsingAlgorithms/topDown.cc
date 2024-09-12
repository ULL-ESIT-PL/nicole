#include "../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"

namespace nicole {
std::unique_ptr<Node> TopDown::parse(const std::filesystem::path& path) const {
  tokens_ = lexer_.analyze(path);
  root_ = std::make_unique<NodeStatement>(std::move(parseStart()));
  return std::move(root_);
}

std::unique_ptr<Node> TopDown::parseStart() const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseFactor()};

  while (std::size_t(currentToke_) < tokens_.size() &&
         (getCurrentToke().type() == TokenType::OPERATOR_ADD ||
          getCurrentToke().type() == TokenType::OPERATOR_SUB)) {
    const Token token{getCurrentToke()};
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
  switch (getCurrentToke().type()) {
    case TokenType::NUMBER_INT: {
      const int value{std::stoi(getCurrentToke().raw())};
      eat();
      return std::make_unique<NodeLiteralInt>(value);
    }
    case TokenType::NUMBER_DOUBLE: {
      const double value{std::stod(getCurrentToke().raw())};
      eat();
      return std::make_unique<NodeLiteralDouble>(value);
    }
    case TokenType::LP: {
      eat();
      auto expression{parseStart()};
      if (getCurrentToke().type() == TokenType::RP) {
        eat();
      } else {
        const std::string strErr{"Error: missing right parenthesis, found " +
                                 getCurrentToke().raw()};
        llvm::report_fatal_error(strErr.c_str());
      }
      return expression;
    }
    default:
      const std::string strErr{"Error: unknown token found " +
                               getCurrentToke().raw()};
      llvm::report_fatal_error(strErr.c_str());
  }
}
}  // namespace nicole
