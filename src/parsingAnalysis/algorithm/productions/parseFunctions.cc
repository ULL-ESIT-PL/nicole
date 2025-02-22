#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <memory>
#include <vector>

namespace nicole {

const std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
TopDown::parseFuncDecl() const noexcept {
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  if (tkStream_.current()->type() != TokenType::ID) {
    return createError(ERROR_TYPE::SINTAX,
                       "missing identifier of function at " +
                           tkStream_.current()->locInfo());
  }
  const Token id{*tkStream_.current()};
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  std::expected<std::vector<GenericParameter>, Error> generics{};
  if (tkStream_.current()->type() == TokenType::OPERATOR_SMALLER) {
    generics = parseGenerics();
  }
  if (!generics) {
    return createError(generics.error());
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
  if (tkStream_.current()->type() != TokenType::DOTDOT) {
    return createError(ERROR_TYPE::SINTAX,
                       "missing : after ) of function decl at " +
                           tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  const std::expected<std::shared_ptr<Type>, Error> returnType{parseType()};
  if (!returnType) {
    return createError(returnType.error());
  }
  const std::expected<std::shared_ptr<AST_BODY>, Error> body{parseBody()};
  if (!body || !*body) {
    return createError(body ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                            : body.error());
  }
  return Builder::createFuncDecl(id.raw(), *generics, *params, *returnType,
                                 *body);
}

const std::expected<Parameters, Error> TopDown::parseParams() const noexcept {
  std::vector<std::pair<std::string, std::shared_ptr<Type>>> params{};
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::RP) {
    if (tkStream_.current()->type() != TokenType::ID) {
      return createError(ERROR_TYPE::SINTAX,
                         "missing id of param of function decl at " +
                             tkStream_.current()->raw() + " at " +
                             tkStream_.current()->locInfo());
    }
    const Token id{*tkStream_.current()};
    if (auto res = tryEat(); !res) {
      return createError(res.error());
    }
    if (tkStream_.current()->type() != TokenType::DOTDOT) {
      return createError(ERROR_TYPE::SINTAX,
                         "missing : after param of function decl at " +
                             tkStream_.current()->raw() + " at " +
                             tkStream_.current()->locInfo());
    }
    if (auto res = tryEat(); !res) {
      return createError(res.error());
    }
    const std::expected<std::shared_ptr<Type>, Error> returnType{parseType()};
    if (!returnType) {
      return createError(returnType.error());
    }
    params.push_back({id.raw(), *returnType});
    if (tkStream_.current()->type() == TokenType::COMMA) {
      if (auto res = tryEat(); !res) {
        return createError(res.error());
      }
      continue;
    } else if (tkStream_.current()->type() != TokenType::RP) {
      return createError(ERROR_TYPE::SINTAX,
                         "missing comma or parenthesis of function decl at " +
                             tkStream_.current()->locInfo());
    }
    break;
  }
  return Parameters{params};
}

const std::expected<std::shared_ptr<AST_RETURN>, Error>
TopDown::parseReturn() const noexcept {
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  if (tkStream_.current()->type() == TokenType::SEMICOLON) {
    return Builder::createReturn(nullptr);
  }
  const std::expected<std::shared_ptr<AST>, Error> value{parseTernary()};
  if (!value || !*value) {
    return createError(value ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                             : value.error());
  }
  if (tkStream_.current()->type() != TokenType::SEMICOLON) {
    return createError(ERROR_TYPE::SINTAX, "missing ; of return at " +
                                               tkStream_.current()->locInfo());
  }
  return Builder::createReturn(*value);
}

} // namespace nicole