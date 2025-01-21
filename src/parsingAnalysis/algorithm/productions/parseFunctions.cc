#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
TopDown::parseFuncDecl() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_RETURN>, Error>
TopDown::parseReturn() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() == TokenType::SEMICOLON) {
    return Builder::createReturn(nullptr);
  }
  const std::expected<std::shared_ptr<AST>, Error> value{parseTernary()};
  if (!value || !*value) {
    return std::unexpected{value ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : value.error()};
  }
  if (tkStream_.current()->type() != TokenType::SEMICOLON) {
    return std::unexpected{Error{
        ERROR_TYPE::SINTAX, "missing ; at " + tkStream_.current()->locInfo()}};
  }
  return Builder::createReturn(*value);
}

} // namespace nicole