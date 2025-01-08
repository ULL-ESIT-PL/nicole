#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseOr() const noexcept {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseAnd()};
  if (!left) {
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         tkStream_.current()->type() == TokenType::OR) {
    const auto token{tkStream_.current()};
    if (!token) {
    }
    if (!tkStream_.eat()) {
    }
    const auto right{parseAnd()};
    if (!right) {
    }
    left = Builder::createOr(*token, *left, *right);
    if (!left) {
    }
  }

  return *left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseAnd() const noexcept {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseEqual_NotEqual()};
  if (!left) {
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         tkStream_.current()->type() == TokenType::OR) {
    const auto token{tkStream_.current()};
    if (!token) {
    }
    if (!tkStream_.eat()) {
    }
    const auto right{parseEqual_NotEqual()};
    if (!right) {
    }
    left = Builder::createAnd(*token, *left, *right);
    if (!left) {
    }
  }

  return *left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseEqual_NotEqual() const noexcept {
  auto left{parseCompare()};
  if (!left) {
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::EQUAL ||
          tkStream_.current()->type() == TokenType::NOTEQUAL)) {
    const auto token{tkStream_.current()};
    if (!token) {
    }
    if (!tkStream_.eat()) {
    }
    const auto right{parseCompare()};
    if (!right) {
    }

    switch (token->type()) {
    case TokenType::EQUAL:
      left = Builder::createEqual(*token, *left, *right);
      break;
    case TokenType::NOTEQUAL:
      left = Builder::createNotEqual(*token, *left, *right);
      break;
    default:
      // error
    }
  }

  if (!left) {
  }

  return left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseCompare() const noexcept {
  auto left{parseAdd_Sub()};
  if (!left) {
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::OPERATOR_SMALLER ||
          tkStream_.current()->type() == TokenType::SMALLEREQUAL ||
          tkStream_.current()->type() == TokenType::OPERATOR_GREATER ||
          tkStream_.current()->type() == TokenType::BIGGEREQUAL)) {
    const auto token{tkStream_.current()};
    if (!token) {
    }
    if (!tkStream_.eat()) {
    }
    const auto right{parseAdd_Sub()};
    if (!right) {
    }
    switch (token->type()) {
    case TokenType::OPERATOR_SMALLER:
      left = Builder::createSmaller(*token, *left, *right);
      break;
    case TokenType::SMALLEREQUAL:
      left = Builder::createSmallerEqual(*token, *left, *right);
      break;
    case TokenType::OPERATOR_GREATER:
      left = Builder::createBigger(*token, *left, *right);
      break;
    case TokenType::BIGGEREQUAL:
      left = Builder::createBiggerEqual(*token, *left, *right);
      break;
    default:
      // error
    }
  }

  if (!left) {
  }

  return left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseAdd_Sub() const noexcept {
  auto left{parseMult_Div_Module()};
  if (!left) {
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::OPERATOR_ADD ||
          tkStream_.current()->type() == TokenType::OPERATOR_SUB)) {
    const auto token{tkStream_.current()};
    if (!token) {
    }
    if (!tkStream_.eat()) {
    }
    const auto right{parseMult_Div_Module()};
    if (!right) {
    }
    switch (token->type()) {
    case TokenType::OPERATOR_ADD:
      left = Builder::createAdd(*token, *left, *right);
      break;
    case TokenType::OPERATOR_SUB:
      left = Builder::createSub(*token, *left, *right);
      break;
    default:
      // ERROR
    }
  }

  if (!left) {
  }

  return left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseMult_Div_Module() const noexcept {
  auto left{parseFactor()};
  if (!left) {
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::OPERATOR_MULT ||
          tkStream_.current()->type() == TokenType::OPERATOR_DIV ||
          tkStream_.current()->type() == TokenType::OPERATOR_MODULE)) {
    const auto token{tkStream_.current()};
    if (!token) {
    }
    if (!tkStream_.eat()) {
    }
    const auto right{parseFactor()};
    if (!right) {
    }
    switch (token->type()) {
    case TokenType::OPERATOR_MULT:
      left = Builder::createMult(*token, *left, *right);
      break;
    case TokenType::OPERATOR_DIV:
      left = Builder::createDiv(*token, *left, *right);
      break;
    case TokenType::OPERATOR_MODULE:
      left = Builder::createModule(*token, *left, *right);
      break;
    default:
      // error
    }
  }

  if (!left) {
  }

  return left;
}

} // namespace nicole