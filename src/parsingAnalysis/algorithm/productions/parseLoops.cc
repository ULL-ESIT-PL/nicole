#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_WHILE>, Error>
TopDown::parseWhile() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() != TokenType::LP) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "missing left parenthesis at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
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
  if (tkStream_.current()->type() != TokenType::RP) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "missing right parenthesis at " +
                                      tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  const std::expected<std::shared_ptr<AST_BODY>, Error> body{parseBody()};
  if (!body || !*body) {
    return std::unexpected{body ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                : body.error()};
  }
  return Builder::createWhile(*condition, *body);
}

const std::expected<std::shared_ptr<AST_FOR>, Error>
TopDown::parseFor() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_DO_WHILE>, Error>
TopDown::parseDoWhile() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  const std::expected<std::shared_ptr<AST_BODY>, Error> body{parseBody()};
  if (!body || !*body) {
    return std::unexpected{body ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                : body.error()};
  }
  if (tkStream_.current()->type() != TokenType::WHILE) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "missing while at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() != TokenType::LP) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "missing left parenthesis at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
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
  if (tkStream_.current()->type() != TokenType::RP) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "missing right parenthesis at " +
                                      tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.current() or tkStream_.current()->type() != TokenType::SEMICOLON) {
    return std::unexpected{Error{
        ERROR_TYPE::SINTAX, "missing ; at "}};
  }
  return Builder::createDoWhile(*body, *condition);
}

} // namespace nicole