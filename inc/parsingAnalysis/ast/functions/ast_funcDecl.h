#ifndef AST_FUNC_DECL_H
#define AST_FUNC_DECL_H

#include "../statements/ast_body.h"
#include "parameters.h"
#include <memory>
#include <string>

namespace nicole {

class AST_FUNC_DECL : public AST {
private:
  std::string id_;
  Parameters params_;
  std::string returnType_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_FUNC_DECL(const std::string &id, const Parameters &params,
                         const std::string &returnType,
                         const std::shared_ptr<AST_BODY> &body) noexcept
      : AST(AST_TYPE::FUNC_DECL), id_{id}, params_{params},
        returnType_{returnType}, body_{body} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const Parameters &parameters() const noexcept {
    return params_;
  }

  [[nodiscard]] const std::string &returnType() const noexcept {
    return returnType_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif