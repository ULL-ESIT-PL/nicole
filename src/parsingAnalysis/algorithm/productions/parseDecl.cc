#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseVarDecl() const noexcept {
  std::expected<Token, Error> token{tkStream_.current()};
  if (!token) {
  }
  if (token->type() == TokenType::LET) {
    if (!tkStream_.eat()) {
    }
    token = tkStream_.current();
    if (token->type() == TokenType::ID) {
      const std::string id{token->raw()};
      if (!tkStream_.eat()) {
      }
      if (tkStream_.current()->type() == TokenType::DOTDOT) {
        if (!tkStream_.eat()) {
        }
      } else {
        // error
      }
      token = tkStream_.current();
      if (token->type() == TokenType::ID) {
        const std::string idTypeStr{token->raw()};
        // std::shared_ptr<GenericType> idType{typeTable_->type(idTypeStr)};
        if (!tkStream_.eat()) {
        }
        if (tkStream_.current()->type() == TokenType::ASSIGNMENT) {
          if (!tkStream_.eat()) {
          }
          auto value{parseTernary()};
          if (!value) {
          }
          return Builder::createLetDecl(id, idTypeStr, *value);
        } else {
          // error
        }

      } else {
        // error
      }
    }
  } else if (token->type() == TokenType::CONST) {
    if (!tkStream_.eat()) {
    }
    token = tkStream_.current();
    if (token->type() == TokenType::ID) {
      const std::string id{token->raw()};
      if (!tkStream_.eat()) {
      }
      if (tkStream_.current()->type() == TokenType::DOTDOT) {
        if (!tkStream_.eat()) {
        }
      } else {
        // error
      }
      token = tkStream_.current();
      if (token->type() == TokenType::ID) {
        const std::string idTypeStr{token->raw()};
        if (!tkStream_.eat()) {
        }
        if (tkStream_.current()->type() == TokenType::ASSIGNMENT) {
          if (!tkStream_.eat()) {
          }
          auto value{parseTernary()};
          if (!value) {
          }
          return Builder::createConstDecl(id, idTypeStr, *value);
        } else {
          // error
        }

      } else {
        // error
      }
    }
  } else if (tkStream_.isCurrentTokenType(TokenType::AUTO)) {
    if (!tkStream_.eat()) {
    }
    token = tkStream_.current();
    std::string id{""};
    if (token->type() == TokenType::ID) {
      id = token->raw();
      if (!tkStream_.eat()) {
      }
    } else {
      // error
    }
    if (tkStream_.current()->type() == TokenType::ASSIGNMENT) {
      if (!tkStream_.eat()) {
      }
      auto value{parseTernary()};
      if (!value) {
      }
      return Builder::createAutoDecl(id, *value);
    } else {
      // error
    }
  }
  const auto ternary{parseTernary()};
  if (!ternary) {
  }
  return ternary;
}

} // namespace nicole