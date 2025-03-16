#ifndef METHOD_H
#define METHOD_H

#include "../../../../parsingAnalysis/ast/functions/parameters.h"
#include "../../../symbol.h"
#include "genericParameter.h"
#include <memory>

namespace nicole {

class Method final : public Symbol {
private:
  std::vector<GenericParameter> generics_;
  Parameters params_;
  std::shared_ptr<Type> returnType_;
  bool isVirtual_;

public:
  explicit Method(const std::string &id,
                  const std::vector<GenericParameter> &generics,
                  const Parameters &params,
                  const std::shared_ptr<Type> &returnType,
                  const bool isVirtual) noexcept
      : Symbol{id}, generics_{generics}, params_{params},
        returnType_{returnType}, isVirtual_{isVirtual} {}

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  [[nodiscard]] const Parameters &params() const noexcept { return params_; }

  [[nodiscard]] const std::shared_ptr<Type> &returnType() const noexcept {
    return returnType_;
  }

  [[nodiscard]] bool isVirtual() const noexcept { return isVirtual_; }

  [[nodiscard]] bool operator==(const Method &other) const noexcept;
};

} // namespace nicole

#endif