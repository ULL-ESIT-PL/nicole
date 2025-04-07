#ifndef METHOD_H
#define METHOD_H

#include "../../../../parsingAnalysis/ast/functions/parameters.h"
#include "../../../symbol.h"
#include "genericParameter.h"
#include <memory>

namespace nicole {

class AST_BODY;

class Method final : public Symbol {
private:
  mutable std::vector<GenericParameter> generics_;
  mutable Parameters params_;
  mutable std::shared_ptr<Type> returnType_;
  std::shared_ptr<AST_BODY> body_;
  bool isVirtual_;
  mutable bool isInherited_{false};

public:
  explicit Method(const std::string &id,
                  const std::vector<GenericParameter> &generics,
                  const Parameters &params,
                  const std::shared_ptr<Type> &returnType,
                  const std::shared_ptr<AST_BODY> &body,
                  const bool isVirtual) noexcept
      : Symbol{id}, generics_{generics}, params_{params},
        returnType_{returnType}, body_{body}, isVirtual_{isVirtual} {}

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  [[nodiscard]] const Parameters &params() const noexcept { return params_; }

  [[nodiscard]] const std::shared_ptr<Type> &returnType() const noexcept {
    return returnType_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }

  [[nodiscard]] bool isVirtual() const noexcept { return isVirtual_; }

  void setInherit(const bool inherit) const noexcept { isInherited_ = inherit; }

  [[nodiscard]] bool isInherited() const noexcept { return isInherited_; }

  void setReturnType(const std::shared_ptr<Type> &type) const noexcept {
    returnType_ = type;
  }

  void setParameters(const Parameters &params) const noexcept {
    params_ = params;
  }

  void
  setGenerics(const std::vector<GenericParameter> &generics) const noexcept {
    generics_ = generics;
  }
};

} // namespace nicole

#endif