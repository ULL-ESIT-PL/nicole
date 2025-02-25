#ifndef FUNCTION_H
#define FUNCTION_H

#include "../../parsingAnalysis/ast/functions/ast_funcDecl.h"

namespace nicole {

class Function final {
private:
  std::string id_;
  std::vector<GenericParameter> generics_;
  Parameters params_;
  std::shared_ptr<Type> returnType_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit Function(const std::string &id,
                    const std::vector<GenericParameter> &generics,
                    const Parameters &params,
                    const std::shared_ptr<Type> &returnType,
                    const std::shared_ptr<AST_BODY> &body) noexcept
      : id_{id}, generics_{generics}, params_{params}, returnType_{returnType},
        body_{body} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

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