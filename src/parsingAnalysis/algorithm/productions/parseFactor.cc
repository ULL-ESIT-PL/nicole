#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <expected>
#include <memory>

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseFactor() const noexcept {
  const std::string raw{tkStream_.current()->raw()};
  const std::string loc{tkStream_.current()->locInfo()};
  switch (tkStream_.current()->type()) {
  case TokenType::TRUE: {
    if (!tkStream_.eat()) {
      break;
    }
    return Builder::createBool(true);
  }

  case TokenType::FALSE: {
    if (!tkStream_.eat()) {
      break;
    }
    return Builder::createBool(false);
  }

  case TokenType::NUMBER_INT: {
    const int value{std::stoi(raw)};
    if (!tkStream_.eat()) {
      break;
    }
    return Builder::createInt(value);
  }

  case TokenType::NUMBER_DOUBLE: {
    const double value{std::stod(raw)};
    if (!tkStream_.eat()) {
      break;
    }
    return Builder::createDouble(value);
  }

  case TokenType::NUMBER_FLOAT: {
    const float value{std::stof(raw.substr(1))};
    if (!tkStream_.eat()) {
      break;
    }
    return Builder::createFloat(value);
  }

  case TokenType::STRING: {
    const std::string value{raw};
    if (!tkStream_.eat()) {
      break;
    }
    return Builder::createString(value);
  }

  case TokenType::CHAR: {
    const std::string value{raw};
    if (!tkStream_.eat()) {
      break;
    }
    return Builder::createChar(value);
  }

  case TokenType::NULLPTR: {
    if (!tkStream_.eat()) {
      break;
    }
    return Builder::createNull();
  }

  case TokenType::ID: {
    return parseChainedExpression();
  }

  case TokenType::LC: {
    return parseVector();
  }

  case TokenType::LP: {
    if (!tkStream_.eat()) {
      break;
    }
    if (tkStream_.current()->type() == TokenType::RP) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "empty expression at " + loc}};
    }
    const std::expected<std::shared_ptr<AST>, Error> expression{parseOr()};
    if (!expression || !*expression) {
      return std::unexpected{expression
                                 ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : expression.error()};
    }
    if (tkStream_.current()->type() != TokenType::RP) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "missing right parenthesis at " +
                                        tkStream_.current()->locInfo()}};
    }
    if (!tkStream_.eat()) {
      break;
    }
    return expression;
  }

  case TokenType::OPERATOR_SUB: {
    const Token token{*tkStream_.current()};
    if (!tkStream_.eat()) {
      break;
    }
    const std::expected<std::shared_ptr<AST>, Error> expression{parseOr()};
    if (!expression || !*expression) {
      return std::unexpected{expression
                                 ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : expression.error()};
    }
    return Builder::createNeg(token, *expression);
  }

  case TokenType::OPERATOR_NOT: {
    const Token token{*tkStream_.current()};
    if (!tkStream_.eat()) {
      break;
    }
    const std::expected<std::shared_ptr<AST>, Error> expression{parseOr()};
    if (!expression || !*expression) {
      return std::unexpected{expression
                                 ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : expression.error()};
    }
    return Builder::createNot(token, *expression);
  }

  case TokenType::DECREMENT: {
    const Token token{*tkStream_.current()};
    if (!tkStream_.eat()) {
      break;
    }
    const std::expected<std::shared_ptr<AST>, Error> expression{parseOr()};
    if (!expression || !*expression) {
      return std::unexpected{expression
                                 ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : expression.error()};
    }
    return Builder::createDecrement(token, *expression);
  }

  case TokenType::INCREMENT: {
    const Token token{*tkStream_.current()};
    if (!tkStream_.eat()) {
      break;
    }
    const std::expected<std::shared_ptr<AST>, Error> expression{parseOr()};
    if (!expression || !*expression) {
      return std::unexpected{expression
                                 ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : expression.error()};
    }
    return Builder::createIncrement(token, *expression);
  }

  default:
    // error
    return std::unexpected<Error>{
        Error{ERROR_TYPE::SINTAX,
              "token " + raw + " is in a wrong position at " + loc}};
  }
  return std::unexpected{
      Error{ERROR_TYPE::SINTAX, "failed to eat " + raw + " at " + loc}};
}

const std::expected<std::shared_ptr<AST_VECTOR>, Error>
TopDown::parseVector() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  std::vector<std::shared_ptr<AST>> expressions{};
  if (tkStream_.current()->type() == TokenType::RC) {
    if (!tkStream_.eat()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                  " at " + tkStream_.current()->locInfo()}};
    }
    return Builder::createVector(expressions);
  }
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::RC) {
    const std::expected<std::shared_ptr<AST>, Error> expression{parseOr()};
    if (!expression || !*expression) {
      return std::unexpected{
          expression ? Error{ERROR_TYPE::NULL_NODE, "node is null"} : expression.error()};
    }
    expressions.push_back(*expression);
    if (tkStream_.current()->type() == TokenType::COMMA) {
      if (!tkStream_.eat()) {
        return std::unexpected{Error{
            ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                    " at " + tkStream_.current()->locInfo()}};
      }
      continue;
    } else if (tkStream_.current()->type() != TokenType::RC) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "missing comma or ] of vector at " +
                                        tkStream_.current()->locInfo()}};
    }
    break;
  }
  if (tkStream_.current()->type() != TokenType::RC) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "missing ] of vector at " +
                                      tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  return Builder::createVector(expressions);
}

const std::expected<std::vector<std::shared_ptr<AST>>, Error>
TopDown::parseArguments(const TokenType end) const noexcept {
  std::vector<std::shared_ptr<AST>> params{};
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != end) {
    const std::expected<std::shared_ptr<AST>, Error> param{parseOr()};
    if (!param || !*param) {
      return std::unexpected{
          param ? Error{ERROR_TYPE::NULL_NODE, "node is null"} : param.error()};
    }
    params.push_back(*param);
    if (tkStream_.current()->type() == TokenType::COMMA) {
      if (!tkStream_.eat()) {
        return std::unexpected{Error{
            ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                    " at " + tkStream_.current()->locInfo()}};
      }
      continue;
    } else if (tkStream_.current()->type() != end) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "missing comma or parenthesis of print at " +
                                        tkStream_.current()->locInfo()}};
    }
    break;
  }
  if (tkStream_.current()->type() != end) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "missing right parenthesis of print at " +
                                      tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  return params;
}

} // namespace nicole