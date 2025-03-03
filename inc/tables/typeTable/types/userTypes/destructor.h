#ifndef DESTRUCTOR_H
#define DESTRUCTOR_H

#include "../../../../parsingAnalysis/ast/functions/parameters.h"
#include "../../../symbol.h"
#include "../specialTypes/voidType.h"
#include "genericParameter.h"
#include <memory>

namespace nicole {

class Destructor final : public Symbol {
private:
  std::vector<GenericParameter> generics_;
  std::shared_ptr<Type> returnType_;

public:
  explicit Destructor(const std::string &id) noexcept
      : Symbol{id}, returnType_{std::make_shared<VoidType>()} {}

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  [[nodiscard]] const std::shared_ptr<Type> &returnType() const noexcept {
    return returnType_;
  }
};

} // namespace nicole

#endif