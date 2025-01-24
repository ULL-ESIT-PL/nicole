#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseVarDecl() const noexcept {
  const Token token{*tkStream_.current()};
  switch (token.type()) {
  case TokenType::CONST: { // const variable: type = expresion;
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
  }
  case TokenType::LET: { // let variable: type = expresion;
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
  }
  case TokenType::AUTO: { // const auto variable: type = expresion;
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
  }
  case TokenType::PTR: { // const ptr variable: type = expresion;
  }
  default: {
    return parseTernary();
  }
  }
}

} // namespace nicole