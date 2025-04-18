#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseAssignment(const bool insideFor) const noexcept {
  const auto firsToken{tkStream_.current()};
  const auto left{parseOr()};
  if (!left || !*left) {
    return createError(left ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                            : left.error());
  }
  const Token token{*tkStream_.current()};
  if (insideFor and
      (token.type() == TokenType::COMMA or token.type() == TokenType::RP)) {
    return left;
  }
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  const auto value{parseOr()};
  if (!value || !*value) {
    return createError(value ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                             : value.error());
  }

  std::expected<std::shared_ptr<AST>, Error> operation{
      Builder::createAssignment(
          SourceLocation{*firsToken, *tkStream_.lastRead()}, token, *left,
          *value)};

  if (!operation || !*operation) {
    return createError(operation ? Error{ERROR_TYPE::NULL_NODE, "right is null"}
                                 : operation.error());
  }

  if (!insideFor and tkStream_.current()->type() != TokenType::SEMICOLON) {
    return createError(ERROR_TYPE::SINTAX,
                       "Expected ';' at the end of assigment expression at " +
                           token.locInfo());
  }

  return operation;
}

} // namespace nicole