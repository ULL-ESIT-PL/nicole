#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseOr() const noexcept {
  std::expected<std::shared_ptr<AST>, Error> left{parseAnd()};

  if (!left || !*left) {
    return std::unexpected{left ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                                : left.error()};
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::OR)) {
    const Token token{*tkStream_.current()};
    std::expected<std::monostate, Error> eaten{tkStream_.eat()};
    if (!eaten) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "failed to eat " + token.raw() + " at " + token.locInfo()}};
    }

    const std::expected<std::shared_ptr<AST>, Error> right{parseAnd()};
    if (!right || !*right) {
      return std::unexpected{right
                                 ? Error{ERROR_TYPE::NULL_NODE,
                                         "right is null at " + token.locInfo()}
                                 : right.error()};
    }

    left = Builder::createBinary(token, *left, *right);
    if (!left) {
      return std::unexpected{left.error()};
    }
  }

  return left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseAnd() const noexcept {
  std::expected<std::shared_ptr<AST>, Error> left{parseEqual_NotEqual()};

  if (!left || !*left) {
    return std::unexpected{left ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                                : left.error()};
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::AND)) {
    const Token token{*tkStream_.current()};
    std::expected<std::monostate, Error> eaten{tkStream_.eat()};
    if (!eaten) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "failed to eat " + token.raw() + " at " + token.locInfo()}};
    }

    const std::expected<std::shared_ptr<AST>, Error> right{
        parseEqual_NotEqual()};
    if (!right || !*right) {
      return std::unexpected{right
                                 ? Error{ERROR_TYPE::NULL_NODE,
                                         "right is null at " + token.locInfo()}
                                 : right.error()};
    }

    left = Builder::createBinary(token, *left, *right);
    if (!left) {
      return std::unexpected{left.error()};
    }
  }

  return left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseEqual_NotEqual() const noexcept {
  std::expected<std::shared_ptr<AST>, Error> left{parseCompare()};

  if (!left || !*left) {
    return std::unexpected{left ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                                : left.error()};
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::EQUAL ||
          tkStream_.current()->type() == TokenType::NOTEQUAL)) {
    const Token token{*tkStream_.current()};
    std::expected<std::monostate, Error> eaten{tkStream_.eat()};
    if (!eaten) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "failed to eat " + token.raw() + " at " + token.locInfo()}};
    }

    const std::expected<std::shared_ptr<AST>, Error> right{parseCompare()};
    if (!right || !*right) {
      return std::unexpected{right
                                 ? Error{ERROR_TYPE::NULL_NODE,
                                         "right is null at " + token.locInfo()}
                                 : right.error()};
    }

    left = Builder::createBinary(token, *left, *right);
    if (!left) {
      return std::unexpected{left.error()};
    }
  }

  return left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseCompare() const noexcept {
  std::expected<std::shared_ptr<AST>, Error> left{parseAdd_Sub()};

  if (!left || !*left) {
    return std::unexpected{left ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                                : left.error()};
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::OPERATOR_SMALLER ||
          tkStream_.current()->type() == TokenType::SMALLEREQUAL ||
          tkStream_.current()->type() == TokenType::BIGGEREQUAL ||
          tkStream_.current()->type() == TokenType::BIGGEREQUAL)) {
    const Token token{*tkStream_.current()};
    std::expected<std::monostate, Error> eaten{tkStream_.eat()};
    if (!eaten) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "failed to eat " + token.raw() + " at " + token.locInfo()}};
    }

    const std::expected<std::shared_ptr<AST>, Error> right{parseAdd_Sub()};
    if (!right || !*right) {
      return std::unexpected{right
                                 ? Error{ERROR_TYPE::NULL_NODE,
                                         "right is null at " + token.locInfo()}
                                 : right.error()};
    }

    left = Builder::createBinary(token, *left, *right);
    if (!left) {
      return std::unexpected{left.error()};
    }
  }

  return left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseAdd_Sub() const noexcept {
  std::expected<std::shared_ptr<AST>, Error> left{parseMult_Div_Module()};

  if (!left || !*left) {
    return std::unexpected{left ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                                : left.error()};
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::OPERATOR_ADD ||
          tkStream_.current()->type() == TokenType::OPERATOR_SUB)) {
    const Token token{*tkStream_.current()};
    std::expected<std::monostate, Error> eaten{tkStream_.eat()};
    if (!eaten) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "failed to eat " + token.raw() + " at " + token.locInfo()}};
    }

    const std::expected<std::shared_ptr<AST>, Error> right{
        parseMult_Div_Module()};
    if (!right || !*right) {
      return std::unexpected{right
                                 ? Error{ERROR_TYPE::NULL_NODE,
                                         "right is null at " + token.locInfo()}
                                 : right.error()};
    }

    left = Builder::createBinary(token, *left, *right);
    if (!left) {
      return std::unexpected{left.error()};
    }
  }

  return left;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseMult_Div_Module() const noexcept {
  std::expected<std::shared_ptr<AST>, Error> left{parseFactor()};

  if (!left || !*left) {
    return std::unexpected{left ? Error{ERROR_TYPE::NULL_NODE, "left is null"}
                                : left.error()};
  }

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current()->type() == TokenType::OPERATOR_MULT ||
          tkStream_.current()->type() == TokenType::OPERATOR_DIV ||
          tkStream_.current()->type() == TokenType::OPERATOR_MODULE)) {
    const Token token{*tkStream_.current()};
    std::expected<std::monostate, Error> eaten{tkStream_.eat()};
    if (!eaten) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "failed to eat " + token.raw() + " at " + token.locInfo()}};
    }

    const std::expected<std::shared_ptr<AST>, Error> right{parseFactor()};
    if (!right || !*right) {
      return std::unexpected{right
                                 ? Error{ERROR_TYPE::NULL_NODE,
                                         "right is null at " + token.locInfo()}
                                 : right.error()};
    }

    left = Builder::createBinary(token, *left, *right);
    if (!left) {
      return std::unexpected{left.error()};
    }
  }

  return left;
}

} // namespace nicole