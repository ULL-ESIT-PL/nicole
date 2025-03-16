#ifndef DESTRUCTOR_H
#define DESTRUCTOR_H

#include "../../../../parsingAnalysis/ast/functions/parameters.h"
#include "../../../symbol.h"
#include "../specialTypes/voidType.h"
#include "genericParameter.h"
#include <memory>

namespace nicole {

class AST_BODY;

class Destructor final : public Symbol {
private:
  std::vector<GenericParameter> generics_;
  std::shared_ptr<Type> returnType_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit Destructor(const std::string &id, const std::shared_ptr<AST_BODY> &body) noexcept
      : Symbol{id}, returnType_{std::make_shared<VoidType>()}, body_{body} {}

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  [[nodiscard]] const std::shared_ptr<Type> &returnType() const noexcept {
    return returnType_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif