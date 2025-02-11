#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_ENUM>, Error>
TopDown::parseEnum() const noexcept {
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  if (tkStream_.current()->type() != TokenType::ID) {
    return createError(ERROR_TYPE::SINTAX, "missing enum identifier at " +
                                               tkStream_.current()->locInfo());
  }
  const Token id{*tkStream_.current()};
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  if (tkStream_.current()->type() != TokenType::LB) {
    return createError(ERROR_TYPE::SINTAX, "missing { of enum at " +
                                               tkStream_.current()->locInfo());
  }
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  std::vector<std::string> identifiers{};
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::RB) {

    if (tkStream_.current()->type() != TokenType::ID) {
      return createError(ERROR_TYPE::SINTAX,
                         "failed to eat " + tkStream_.current()->raw() +
                             " at " + tkStream_.current()->locInfo());
    }
    identifiers.push_back(tkStream_.current()->raw());
    if (auto res = tryEat(); !res) {
      return createError(res.error());
    }
    if (tkStream_.current()->type() == TokenType::COMMA) {
      if (auto res = tryEat(); !res) {
        return createError(res.error());
      }
      continue;
    } else if (tkStream_.current()->type() != TokenType::RB) {
      return createError(ERROR_TYPE::SINTAX,
                         "missing comma or parenthesis of enum at " +
                             tkStream_.current()->locInfo());
    }
    break;
  }
  if (tkStream_.current()->type() != TokenType::RB) {
    return createError(ERROR_TYPE::SINTAX, "missing } of enum at " +
                                               tkStream_.current()->locInfo());
  }
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  return Builder::createEnum(id.raw(), identifiers);
}

const std::expected<std::shared_ptr<AST_CLASS>, Error>
TopDown::parseClassDecl() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_STRUCT>, Error>
TopDown::parseStructDecl() const noexcept {
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  if (tkStream_.current()->type() != TokenType::ID) {
    return createError(ERROR_TYPE::SINTAX, "missing struct identifier at " +
                                               tkStream_.current()->locInfo());
  }
  const Token id{*tkStream_.current()};
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  std::unique_ptr<std::string> fatherType{nullptr};
  if (tkStream_.current()->type() == TokenType::EXTENDS and
      tkStream_.lookAhead(1)->type() == TokenType::ID) {
    if (auto res = tryEat(); !res) {
      return createError(res.error());
    }
    const Token fatherTypeToken{*tkStream_.current()};
    if (auto res = tryEat(); !res) {
      return createError(res.error());
    }
    fatherType = std::make_unique<std::string>(fatherTypeToken.raw());
  }

  if (tkStream_.current()->type() != TokenType::LB) {
    return createError(ERROR_TYPE::SINTAX, "missing { of struct at " +
                                               tkStream_.current()->locInfo());
  }
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }

  bool isConstructorParsed{false};
  bool isDestructorParsed{false};
  std::vector<std::shared_ptr<AST_FUNC_DECL>> methods{};
  std::vector<std::pair<std::string, std::string>> params{};
  std::expected<std::shared_ptr<AST_FUNC_DECL>, Error> constructor{nullptr};
  std::expected<std::shared_ptr<AST_FUNC_DECL>, Error> destructor{nullptr};

  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::RB) {
    switch (tkStream_.current()->type()) {

    case TokenType::ID: {
      const Token attrID{*tkStream_.current()};
      if (auto res = tryEat(); !res) {
        return createError(res.error());
      }
      if (tkStream_.current()->type() != TokenType::DOTDOT) {
        return createError(ERROR_TYPE::SINTAX,
                           "missing : of attr at " +
                               tkStream_.current()->locInfo());
      }
      if (auto res = tryEat(); !res) {
        return createError(res.error());
      }
      if (tkStream_.current()->type() != TokenType::ID) {
        return createError(ERROR_TYPE::SINTAX,
                           "missing type of attr at " +
                               tkStream_.current()->locInfo());
      }
      const Token attrType{*tkStream_.current()};
      if (auto res = tryEat(); !res) {
        return createError(res.error());
      }
      params.push_back({attrID.raw(), attrType.raw()});
      break;
    }

    case TokenType::FUNCTION: {
      const std::expected<std::shared_ptr<AST_FUNC_DECL>, Error> method{
          parseFuncDecl(true)};
      if (!method || !*method) {
        return createError(method ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                  : method.error());
      }
      methods.push_back(*method);
      break;
    }

    case TokenType::CONSTRUCTOR: {
      if (isConstructorParsed) {
        return createError(ERROR_TYPE::SINTAX,
                           "constructor overloading not implemented, at " +
                               tkStream_.current()->locInfo());
      }
      isConstructorParsed = true;
      constructor = parseConstructorDecl(id.raw());
      if (!constructor || !*constructor) {
        return createError(constructor
                               ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                               : constructor.error());
      }
      break;
    }

    case TokenType::DESTRUCTOR: {
      if (isDestructorParsed) {
        return createError(ERROR_TYPE::SINTAX,
                           "destructor overloading not implemented, at " +
                               tkStream_.current()->locInfo());
      }
      isDestructorParsed = true;
      destructor = parseDestructorDecl();
      if (!destructor || !*destructor) {
        return createError(destructor
                               ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                               : destructor.error());
      }
      break;
    }

    default: {
      if (tkStream_.current()->type() != TokenType::RB) {
        return createError(ERROR_TYPE::SINTAX,
                           "token in wrong position at " +
                               tkStream_.current()->locInfo());
      }
    }
    }
  }

  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  return Builder::createStruct(id.raw(), std::move(fatherType),
                               Attributes{params}, methods, *constructor,
                               *destructor);
}

const std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
TopDown::parseConstructorDecl(const std::string &returnType) const noexcept {
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  if (tkStream_.current()->type() != TokenType::LP) {
    return createError(ERROR_TYPE::SINTAX, "missing ( of function at " +
                                               tkStream_.current()->locInfo());
  }
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  const std::expected<Parameters, Error> params{parseParams()};
  if (!params) {
    return createError(params.error());
  }
  if (tkStream_.current()->type() != TokenType::RP) {
    return createError(ERROR_TYPE::SINTAX, "missing ) of function at " +
                                               tkStream_.current()->locInfo());
  }
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  const std::expected<std::shared_ptr<AST_BODY>, Error> body{parseBody()};
  if (!body || !*body) {
    return createError(body ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                            : body.error());
  }
  return Builder::createFuncDecl("constructor", *params, returnType, *body,
                                 true);
}

const std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
TopDown::parseDestructorDecl() const noexcept {
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  const auto body{parseBody()};
  if (!body || !*body) {
    return createError(body ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                            : body.error());
  }
  return Builder::createFuncDecl("destructor", Parameters{{}}, "void", *body,
                                 true);
}

} // namespace nicole