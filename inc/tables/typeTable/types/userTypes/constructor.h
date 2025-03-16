#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include "../../../../parsingAnalysis/ast/functions/parameters.h"
#include "../../../symbol.h"
#include "genericParameter.h"
#include <memory>

namespace nicole {

class AST_BODY;

class Constructor final : public Symbol {
private:
  std::vector<GenericParameter> generics_;
  Parameters params_;
  std::shared_ptr<Type> returnType_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit Constructor(const std::string &id,
                       const std::vector<GenericParameter> &generics,
                       const Parameters &params,
                       const std::shared_ptr<Type> &returnType,
                       const std::shared_ptr<AST_BODY> &body) noexcept
      : Symbol{id}, generics_{generics}, params_{params},
        returnType_{returnType}, body_{body} {}

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
};

} // namespace nicole

#endif