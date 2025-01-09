#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <expected>

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseFactor() const noexcept {
  switch (tkStream_.current()->type()) {
  case TokenType::NUMBER_INT: {
    const int value{std::stoi(tkStream_.current()->raw())};
    if (!tkStream_.eat()) {
    }
    return Builder::createInt(value);
  }
  case TokenType::NUMBER_DOUBLE: {
    const double value{std::stod(tkStream_.current()->raw())};
    if (!tkStream_.eat()) {
    }
    return Builder::createDouble(value);
  }
  case TokenType::NUMBER_FLOAT: {
    const float value{std::stof(tkStream_.current()->raw().substr(1))};
    if (!tkStream_.eat()) {
    }
    return Builder::createFloat(value);
  }
  case TokenType::STRING: {
    const std::string value{tkStream_.current()->raw()};
    if (!tkStream_.eat()) {
    }
    return Builder::createString(value);
  }
  case TokenType::CHAR: {
    const std::string value{tkStream_.current()->raw()};
    if (!tkStream_.eat()) {
    }
    return Builder::createChar(value);
  }
  case TokenType::TRUE: {
    if (!tkStream_.eat()) {
    }
    return Builder::createBool(true);
  }
  case TokenType::FALSE: {
    if (!tkStream_.eat()) {
    }
    return Builder::createBool(false);
  }
  case TokenType::OPERATOR_NOT: {
    const auto token{tkStream_.current()};
    if (!tkStream_.eat()) {
    }
    const auto expression{parseOr()};
    if (!expression) {
    }
    return Builder::createNot(*token, *expression);
  }
  case TokenType::OPERATOR_SUB: {
    const auto token{tkStream_.current()};
    if (!tkStream_.eat()) {
    }
    const auto expression{parseOr()};
    if (!expression) {
    }
    return Builder::createNeg(*token, *expression);
  }
  case TokenType::INCREMENT: {
    const auto token{tkStream_.current()};
    if (!tkStream_.eat()) {
    }
    const auto expression{parseOr()};
    if (!expression) {
    }
    return Builder::createIncrement(*token, *expression);
  }
  case TokenType::DECREMENT: {
    const auto token{tkStream_.current()};
    if (!tkStream_.eat()) {
    }
    const auto expression{parseOr()};
    if (!expression) {
    }
    return Builder::createDecrement(*token, *expression);
  }
  case TokenType::ID: {
    return nullptr;
    /*
    const std::string id{tkStream_.current()->raw()};
    if (!tkStream_.eat()) {
    }
    if (tkStream_.current().type() == TokenType::ASSIGNMENT) {
      if (!tkStream_.eat()) {
      }
      auto expression{parseLogicalOr(currentScope, father)};
      return ASTBuilder::createVarRGT(id, expression, currentScope, typeTable_);
    } else if (tkStream_.current().type() == TokenType::LB) {
      if (!tkStream_.eat()) {
      }
      auto attributes{parseParamsCall(currentScope, father)};
      if (tkStream_.current().type() == TokenType::RB) {
        if (!tkStream_.eat()) {
        }
        return ASTBuilder::createStructConstr(id, attributes, typeTable_);
      }
    } else if (tkStream_.current().type() == TokenType::DOT) {
      if (!tkStream_.eat()) {
      }
      if (tkStream_.current().type() == TokenType::ID) {
        const auto attribute{tkStream_.current().raw()};
        if (!tkStream_.eat()) {
        }
        if (tkStream_.current().type() == TokenType::ASSIGNMENT) {
          if (!tkStream_.eat()) {
          }
          auto expression{parseLogicalOr(currentScope, father)};
          return ASTBuilder::createStructSetAttr(id, attribute, expression,
                                                 currentScope, typeTable_);
        }
        return ASTBuilder::createStructAcces(id, attribute, currentScope,
                                             typeTable_);
      } else {
        const std::string strErr{"Error: accessing attribute, found " +
                                 tkStream_.current().raw() + " at" +
                                 tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
    } else if (tkStream_.current().type() == TokenType::LP) {
      return parseFunctionCall(id, currentScope, father);
    }
    return ASTBuilder::createVarCall(id, currentScope);
    */
  }
  case TokenType::STOP: {
    if (!tkStream_.eat()) {
    }
    return Builder::createStop(nullptr);
  }
  case TokenType::PASS: {
    if (!tkStream_.eat()) {
    }
    return Builder::createPass(nullptr);
  }
  case TokenType::LP: {
    if (!tkStream_.eat()) {
    }
    auto expression{parseOr()};
    if (!expression) {
    }
    if (tkStream_.current()->type() == TokenType::RP) {
      if (!tkStream_.eat()) {
      }
    } else {
      // error
    }
    return expression;
  }
  default:
    // error
    return std::unexpected<Error>{Error{ERROR_TYPE::UNKOWN_ARGUMENT, ""}};
  }
}

const std::expected<std::shared_ptr<AST_VECTOR>, Error>
TopDown::parseVector() const noexcept {
  return nullptr;
}

const std::expected<Parameters, Error> TopDown::parseParams() const noexcept {
  std::vector<std::pair<std::string, std::string>> params{};
  if (tkStream_.current()->type() == TokenType::RP) {
    return Parameters{{}};
  }
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    auto token{tkStream_.current()};
    std::string id{};
    std::string type{""};
    if (token->type() == TokenType::ID) {
      id = token->raw();
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
    if (token->type() == TokenType::ID) {
      type = token->raw();
      if (!tkStream_.eat()) {
      }
    } else {
      // error
    }
    params.push_back({id, type});
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::COMMA)) {
      if (!tkStream_.eat()) {
      }
    } else {
      break;
    }
  }
  return Parameters{params};
}

const std::expected<std::vector<std::shared_ptr<AST>>, Error>
TopDown::parseArguments() const noexcept {
  std::vector<std::shared_ptr<AST>> params{};
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    const auto expression{parseTernary()};
    if (!expression) {
    }
    params.push_back(*expression);
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::COMMA)) {
      if (!tkStream_.eat()) {
      }
    } else {
      break;
    }
  }
  return params;
}

} // namespace nicole