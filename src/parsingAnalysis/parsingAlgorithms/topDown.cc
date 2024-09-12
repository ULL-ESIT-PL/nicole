#include "../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"

namespace nicole {
std::unique_ptr<Node> TopDown::parse(const std::filesystem::path& path) const {
  tokens_ = lexer_.analyze(path);
  std::cout << "Size: " << tokens_.size() << '\n';
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
        left = std::make_unique<NodeBinary>(
            std::move(left), TokenType::OPERATOR_ADD, std::move(right));
        break;
      case TokenType::OPERATOR_SUB:
        left = std::make_unique<NodeBinary>(
            std::move(left), TokenType::OPERATOR_SUB, std::move(right));
        break;
      default:
        assert(1 > 2 && "Boo");
    }
    // recursively goes down in the second e
    // left = std::make_unique<NodeBinary>(std::move(left), Operator::ADD,
    //                                   std::move(right));
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
        assert(1 < 0 && "Unkow token2");
      }
      return expression;
    }
    default:
      std::cout << getCurrentToke().raw() << std::flush;
      assert(1 < 0 && "Unkow token");
      break;
  }
}
}  // namespace nicole
