#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_IF>, Error>
TopDown::parseIf() const noexcept {
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
  const auto ifBody{parseBody()};
  if (!ifBody) {
  }

  std::vector<std::shared_ptr<AST_ELSE_IF>> elseIfs{};
  while (tkStream_.isCurrentTokenType(TokenType::ELSE) &&
         tkStream_.lookAhead(1)->type() == TokenType::IF) {
    const auto elseIf{parseElseIf()};
    if (!elseIf) {
    }
    elseIfs.push_back(*elseIf);
  }

  std::expected<std::shared_ptr<AST_BODY>, Error> elseBody;
  if (tkStream_.currentPos() < tkStream_.size() and
      tkStream_.current()->type() == TokenType::ELSE) {
    if (!tkStream_.eat()) {
    }
    elseBody = parseBody();
    if (!elseBody) {
    }
  }

  return Builder::createIf(*condition, *ifBody, elseIfs, *elseBody, sourceStub);
}

const std::expected<std::shared_ptr<AST_ELSE_IF>, Error>
TopDown::parseElseIf() const noexcept {
  if (!tkStream_.eat()) {
  } // else
  if (!tkStream_.eat()) {
  } // if
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
  const auto ifBody{parseBody()};
  if (!ifBody) {
  }
  return Builder::createElseIf(*condition, *ifBody, sourceStub);
}

const std::expected<std::shared_ptr<AST_SWITCH>, Error>
TopDown::parseSwitch() const noexcept {
  if (!tkStream_.eat()) {
  }
  if (tkStream_.isCurrentTokenType(TokenType::LP)) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  if (tkStream_.isCurrentTokenType(TokenType::RP)) {
    // error
  }
  auto match{parseOr()};
  if (!match) {
  }
  if (tkStream_.isCurrentTokenType(TokenType::RP)) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  if (tkStream_.isCurrentTokenType(TokenType::LB)) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  if (tkStream_.isCurrentTokenType(TokenType::RB)) {
    // error
  }
  std::vector<std::shared_ptr<AST_CASE>> cases{};
  while (!tkStream_.isCurrentTokenType(TokenType::DEFAULT) &&
         !tkStream_.isCurrentTokenType(TokenType::RB)) {
    const auto case_{parseCase()};
    if (!case_) {
    }
    cases.push_back(*case_);
  }
  auto defaultCase{parseDefault()};
  if (tkStream_.isCurrentTokenType(TokenType::RB)) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }

  const auto astSwitch{
      Builder::createSwitch(*match, cases, *defaultCase, sourceStub)};
  if (!astSwitch) {
  }
  return astSwitch;
  /*
  switch (true) {
    case 0: {

    }
    default: {

    }
  }
  */
}

const std::expected<std::shared_ptr<AST_CASE>, Error>
TopDown::parseCase() const noexcept {
  if (tkStream_.isCurrentTokenType(TokenType::CASE)) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }

  auto match{parseOr()};

  if (tkStream_.isCurrentTokenType(TokenType::DOTDOT)) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }

  const auto body{parseBody()};
  if (!body) {
  }
  return Builder::createCase(*match, *body, sourceStub);
}

const std::expected<std::shared_ptr<AST_DEFAULT>, Error>
TopDown::parseDefault() const noexcept {
  if (tkStream_.isCurrentTokenType(TokenType::DEFAULT)) {
    if (!tkStream_.eat()) {
    }
  } else if (tkStream_.isCurrentTokenType(TokenType::RB)) {
    return nullptr;
  } else {
    // error
  }

  if (tkStream_.isCurrentTokenType(TokenType::DOTDOT)) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }

  const auto body{parseBody()};
  if (!body) {
  }
  return Builder::createDefault(*body, sourceStub);
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseTernary() const noexcept {
  // To break ambiguous situation we need to check for ? before we even process
  // it
  if (tkStream_.isCurrentTokenType(TokenType::LP) and
      tkStream_.isTokenAheadBeforeSemicolon(TokenType::TERNARY)) {
    if (!tkStream_.eat()) {
    }
    if (tkStream_.isCurrentTokenType(TokenType::RP)) {
      // ERROR
    }
    auto condition{parseOr()};
    if (!condition) {
    }
    if (tkStream_.isCurrentTokenType(TokenType::RP)) {
      if (!tkStream_.eat()) {
      }
    } else {
      // error
    }
    if (tkStream_.isCurrentTokenType(TokenType::TERNARY)) {
      if (!tkStream_.eat()) {
      }
    } else {
      // error
    }
    const auto first{parseOr()};
    if (!first) {
    }
    if (tkStream_.isCurrentTokenType(TokenType::DOTDOT)) {
      if (!tkStream_.eat()) {
      }
    } else {
      // error
    }
    const auto second{parseOr()};
    if (!second) {
    }
    return Builder::createTernary(*condition, *first, *second, sourceStub);
  }
  return parseOr();
}

} // namespace nicole