#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_IF>, Error>
TopDown::parseIf() const noexcept {
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
  const std::expected<std::shared_ptr<AST>, Error> conditionIf{parseOr()};
  if (!conditionIf || !*conditionIf) {
    return std::unexpected{conditionIf
                               ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                               : conditionIf.error()};
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
  const std::expected<std::shared_ptr<AST_BODY>, Error> bodyIf{parseBody()};
  if (!bodyIf || !*bodyIf) {
    return std::unexpected{bodyIf ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                  : bodyIf.error()};
  }
  std::vector<std::shared_ptr<AST_ELSE_IF>> elseIfs{};
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() == TokenType::ELSE and
         tkStream_.lookAhead(1)->type() == TokenType::IF) {
    const std::expected<std::shared_ptr<AST_ELSE_IF>, Error> elseIf{
        parseElseIf()};
    if (!elseIf || !*elseIf) {
      return std::unexpected{
          elseIf ? Error{ERROR_TYPE::NULL_NODE, "Statement is null"}
                 : elseIf.error()};
    }
    elseIfs.push_back(*elseIf);
  }
  if (tkStream_.current()->type() != TokenType::ELSE) {
    return Builder::createIf(*conditionIf, *bodyIf, elseIfs, nullptr);
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  const std::expected<std::shared_ptr<AST_BODY>, Error> bodyElse{parseBody()};
  if (!bodyElse || !*bodyElse) {
    return std::unexpected{bodyElse
                               ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                               : bodyElse.error()};
  }
  return Builder::createIf(*conditionIf, *bodyIf, elseIfs, *bodyElse);
}

const std::expected<std::shared_ptr<AST_ELSE_IF>, Error>
TopDown::parseElseIf() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
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
  const std::expected<std::shared_ptr<AST_BODY>, Error> body{parseBody()};
  if (!body || !*body) {
    return std::unexpected{body ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                : body.error()};
  }
  return Builder::createElseIf(*condition, *body);
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
  // to avoid ambigious situations like (something) being treated like ternary
  if (tkStream_.current()->type() == TokenType::LP and
      tkStream_.isTokenAheadBeforeSemicolon(TokenType::TERNARY)) {
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
    if (tkStream_.current()->type() != TokenType::RP) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "missing right parenthesis at " +
                                        tkStream_.current()->locInfo()}};
    }
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    if (tkStream_.current()->type() != TokenType::TERNARY) {
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
    if (tkStream_.current()->type() != TokenType::DOTDOT) {
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