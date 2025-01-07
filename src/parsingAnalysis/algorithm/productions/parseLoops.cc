#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_WHILE>, Error>
TopDown::parseWhile() const noexcept {
  if (!tkStream_.eat()) {
  }
  if (tkStream_.current()->type() == TokenType::LP) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  if (tkStream_.current()->type() == TokenType::RP) {
    // error
  }

  auto condition{parseOr()};
  if (!condition) {
  }

  if (tkStream_.current()->type() == TokenType::RP) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  auto whileBody{parseBody()};
  if (!whileBody) {
  }

  return Builder::createWhile(*condition, *whileBody, sourceStub);
}

const std::expected<std::shared_ptr<AST_FOR>, Error>
TopDown::parseFor() const noexcept {
  if (!tkStream_.eat()) {
  }
  if (tkStream_.current()->type() == TokenType::LP) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  if (tkStream_.current()->type() == TokenType::RP) {
    // error
  }
  const auto init{parseComma()};
  if (!init) {
  }
  if (tkStream_.current()->type() == TokenType::SEMICOLON) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  const auto condition{parseOr()};
  if (!condition) {
  }
  if (tkStream_.current()->type() == TokenType::SEMICOLON) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  const auto update{parseComma()};
  if (!update) {
  }
  if (tkStream_.current()->type() == TokenType::RP) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  const auto body{parseBody()};
  if (!body) {
  }
  return Builder::createFor(*init, *condition, *update, *body, sourceStub);
}

const std::expected<std::shared_ptr<AST_DO_WHILE>, Error>
TopDown::parseDoWhile() const noexcept {
  if (!tkStream_.eat()) {
  }
  const auto body{parseBody()};
  if (!body) {
  }

  if (tkStream_.current()->type() == TokenType::WHILE) {
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
  if (tkStream_.current()->type() == TokenType::RP) {
    // error
  }
  const auto condition{parseOr()};
  if (!condition) {
  }

  if (tkStream_.current()->type() == TokenType::RP) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  return Builder::createDoWhile(*body, *condition, sourceStub);
}

} // namespace nicole