#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <memory>

namespace nicole {

const std::expected<std::shared_ptr<AST_ENUM>, Error>
TopDown::parseEnum() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() != TokenType::ID) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "missing enum identifier at " + tkStream_.current()->locInfo()}};
  }
  const Token id{*tkStream_.current()};
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() != TokenType::LB) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "missing { of enum at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  std::vector<std::string> identifiers{};
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::RB) {
    if (tkStream_.current()->type() != TokenType::ID) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    identifiers.push_back(tkStream_.current()->raw());
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    if (tkStream_.current()->type() == TokenType::COMMA) {
      if (!tkStream_.eat()) {
        return std::unexpected{Error{
            ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                    " at " + tkStream_.current()->locInfo()}};
      }
      continue;
    } else if (tkStream_.current()->type() != TokenType::RB) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "missing comma or parenthesis of enum at " +
                                        tkStream_.current()->locInfo()}};
    }
    break;
  }
  if (tkStream_.current()->type() != TokenType::RB) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "missing } of enum at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  return Builder::createEnum(id.raw(), identifiers);
}

const std::expected<std::shared_ptr<AST_CLASS>, Error>
TopDown::parseClassDecl() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_STRUCT>, Error>
TopDown::parseStructDecl() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() != TokenType::ID) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "missing struct identifier at " +
                                      tkStream_.current()->locInfo()}};
  }
  const Token id{*tkStream_.current()};
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  std::unique_ptr<std::string> fatherType{nullptr};
  if (tkStream_.current()->type() == TokenType::EXTENDS and
      tkStream_.lookAhead(1)->type() == TokenType::ID) {
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    const Token fatherTypeToken{*tkStream_.current()};
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    fatherType = std::make_unique<std::string>(fatherTypeToken.raw());
  }

  if (tkStream_.current()->type() != TokenType::LB) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "missing { of struct at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }

  bool isConstructorParsed{false};
  bool isDestructorParsed{false};

  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::RB) {
    switch (tkStream_.current()->type()) {

    case TokenType::ATTR: {
    }

    case TokenType::METHOD: {
    }

    case TokenType::CONSTRUCTOR: {
      if (isConstructorParsed) {
      }
    }

    case TokenType::DESTRUCTOR: {
      if (isDestructorParsed) {
      }
    }

    default: {
    }
    }
  }

  return nullptr;
}

} // namespace nicole