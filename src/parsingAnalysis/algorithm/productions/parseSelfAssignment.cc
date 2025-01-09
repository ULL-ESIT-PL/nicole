#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseSelfAssignment() const noexcept {
  const auto tk{tkStream_.current()};
  if (!tkStream_.eat()) {
  }
  const auto op{tkStream_.current()};
  if (!tkStream_.eat()) {
  }
  const auto expression{parseTernary()};
  if (!expression) {
  }
  switch (op->type()) {
  case TokenType::SELF_ADD: {
    return Builder::createSelfAdd(tk->raw(), *expression);
  }
  case TokenType::SELF_SUB: {
    return Builder::createSelfSub(tk->raw(), *expression);
  }
  case TokenType::SELF_MULT: {
    return Builder::createSelfMult(tk->raw(), *expression);
  }
  case TokenType::SELF_DIV: {
    return Builder::createSelfDiv(tk->raw(), *expression);
  }
  default: {
    return std::unexpected<Error>{Error{ERROR_TYPE::UNKOWN_ARGUMENT, ""}};
  }
  }
}

} // namespace nicole