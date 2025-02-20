#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <vector>

namespace nicole {

const std::expected<std::shared_ptr<Type>, Error>
TopDown::parseType() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<Type>, Error>
TopDown::parseTypeAtVarDecl(const bool isConst) const noexcept {
  if (isConst) {
  }
  return nullptr;
}

const std::expected<std::vector<GenericParameter>, Error>
TopDown::parseGenerics() const noexcept {
  std::vector<GenericParameter> result{};
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::OPERATOR_GREATER) {
    if (tkStream_.current()->type() != TokenType::ID) {
      return createError(ERROR_TYPE::SINTAX,
                         "misplaced token " + tkStream_.current()->raw() +
                             " at " + tkStream_.current()->locInfo());
    }
    result.push_back(GenericParameter{tkStream_.current()->raw()});
    if (auto res = tryEat(); !res) {
      return createError(res.error());
    }
    if (tkStream_.current()->type() == TokenType::COMMA) {
      if (auto res = tryEat(); !res) {
        return createError(res.error());
      }
      continue;
    }
  }
  if (!result.size()) {
    return createError(ERROR_TYPE::SINTAX,
                       "no generic type has been specified at" +
                           tkStream_.current()->locInfo());
  }
  if (auto res = tryEat(); !res) {
    return createError(res.error());
  }
  return result;
}

} // namespace nicole

// const name: type = value;
// let name: type = value;
// def fun<Generics>(id: type): type {}
// struct name: extends type<Generics> {}
// constructorDecl<Generics>(id: type) {}
// constructorCall<Generics>(){}
// funCall<Generics>()()