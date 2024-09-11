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
         (getCurrentToke().type() == TokenType::OPERATOR)) {
    // std::string op{getCurrentToke().raw()};
    eat();
    // recursively goes down in the second e
    auto right{parseFactor()};
    left = std::make_unique<NodeBinary>(std::move(left), Operator::ADD,
                                        std::move(right));
  }

  return left;
}

std::unique_ptr<Node> TopDown::parseFactor() const {
  if (getCurrentToke().type() == TokenType::NUMBER_INT) {
    const int value{std::stoi(getCurrentToke().raw())};
    eat();
    return std::make_unique<NodeLiteralInt>(value);
  }
  std::cout << getCurrentToke().raw() << std::flush;
  assert(1 < 0 && "Unkow token");
}
}  // namespace nicole
