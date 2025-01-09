#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
TopDown::parseFuncDecl() const noexcept {
  if (!tkStream_.eat()) {
  }
  auto token{tkStream_.current()};
  std::string id{};
  if (token->type() == TokenType::ID) {
    id = token->raw();
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  if (tkStream_.current()->type() == TokenType::LP) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  std::expected<Parameters, Error> params{parseParams()};
  if (!params) {
  }
  if (tkStream_.current()->type() == TokenType::RP) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  if (tkStream_.current()->type() == TokenType::DOTDOT) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  token = tkStream_.current();
  if (!token) {
  }
  std::string returnType{};
  if (token->type() == TokenType::ID) {
    returnType = token->raw();
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  const auto body{parseBody()};
  if (!body) {
  }
  return Builder::createFuncDecl(id, *params, returnType, *body);
}

const std::expected<std::shared_ptr<AST_RETURN>, Error>
TopDown::parseReturn() const noexcept {
  if (!tkStream_.eat()) {
  }
  if (tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    return Builder::createReturn(nullptr);
  }
  const auto ternary{parseTernary()};
  if (!ternary) {
  }
  return Builder::createReturn(*ternary);
}

} // namespace nicole