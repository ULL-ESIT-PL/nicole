#ifndef AST_SUBRUTINE_DECL_H
#define AST_SUBRUTINE_DECL_H

#include "../statements/ast_body.h"
#include "../../types/type.h"
#include <memory>
#include <string>

namespace nicole {

class AST_SUBRUTINE_DECL : public AST {
private:
  std::string id_;
  std::shared_ptr<Type> returnType_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_SUBRUTINE_DECL(const AST_TYPE type, const std::string &id,
                              const std::shared_ptr<Type> &returnType,
                              const std::shared_ptr<AST_BODY> &body) noexcept
      : AST(type), id_{id}, returnType_{returnType}, body_{body} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::shared_ptr<Type> &returnType() const noexcept {
    return returnType_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif