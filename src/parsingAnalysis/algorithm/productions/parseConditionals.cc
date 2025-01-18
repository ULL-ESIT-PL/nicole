#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_IF>, Error>
TopDown::parseIf() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_ELSE_IF>, Error>
TopDown::parseElseIf() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_SWITCH>, Error>
TopDown::parseSwitch() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_CASE>, Error>
TopDown::parseCase() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_DEFAULT>, Error>
TopDown::parseDefault() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseTernary() const noexcept {
  if (tkStream_.current()->type() == TokenType::LP) {
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    if (tkStream_.current()->type() == TokenType::RP) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "empty expression at " + tkStream_.current()->locInfo()}};
    }
    const std::expected<std::shared_ptr<AST>, Error> condition{parseOr()};
    if (!condition || !*condition) {
      return std::unexpected{condition
                                 ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : condition.error()};
    }
    if (!(tkStream_.current()->type() == TokenType::RP)) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "missing right parenthesis at " +
                                        tkStream_.current()->locInfo()}};
    }
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    if (!(tkStream_.current()->type() == TokenType::TERNARY)) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "missing ? at " + tkStream_.current()->locInfo()}};
    }
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    const std::expected<std::shared_ptr<AST>, Error> first{parseOr()};
    if (!first || !*first) {
      return std::unexpected{
          first ? Error{ERROR_TYPE::NULL_NODE, "node is null"} : first.error()};
    }
    if (!(tkStream_.current()->type() == TokenType::DOTDOT)) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "missing : at " + tkStream_.current()->locInfo()}};
    }
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    const std::expected<std::shared_ptr<AST>, Error> second{parseOr()};
    if (!second || !*second) {
      return std::unexpected{second
                                 ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : second.error()};
    }
    return Builder::createTernary(*condition, *first, *second);
  }
  return parseOr();
}

} // namespace nicole