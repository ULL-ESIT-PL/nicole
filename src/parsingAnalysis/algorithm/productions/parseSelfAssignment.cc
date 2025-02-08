#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseSelfAssignment(const bool insideFor) const noexcept {
  const auto left{parseOr()};
  if (!left || !*left) {
    return std::unexpected{left ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                                : left.error()};
  }
  const Token token{*tkStream_.current()};
  if (insideFor and
      (token.type() == TokenType::COMMA or token.type() == TokenType::RP)) {
    return left;
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  const auto value{parseOr()};
  if (!value || !*value) {
    return std::unexpected{value ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                                 : value.error()};
  }

  std::expected<std::shared_ptr<AST>, Error> operation{nullptr};

  switch (token.type()) {

  case TokenType::ASSIGNMENT: {
    operation = Builder::createAssignment(*left, *value);
    break;
  }

  default: {
    operation = Builder::createSelfAssignment(token, *left, *value);
    break;
  }
  }

  if (!operation || !*operation) {
    return std::unexpected{operation
                               ? Error{ERROR_TYPE::NULL_NODE, "right is null"}
                               : operation.error()};
  }

  if (!insideFor and tkStream_.current()->type() != TokenType::SEMICOLON) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "Expected ';' at the end of assigment expression at " +
                  token.locInfo()}};
  }

  return operation;
}

} // namespace nicole