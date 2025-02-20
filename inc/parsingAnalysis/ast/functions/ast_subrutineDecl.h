#ifndef AST_SUBRUTINE_DECL_H
#define AST_SUBRUTINE_DECL_H

#include "../statements/ast_body.h"
#include <memory>
#include <string>

namespace nicole {

class AST_SUBRUTINE_DECL : public AST {
private:
  std::string id_;
  std::string returnType_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_SUBRUTINE_DECL(const AST_TYPE type, const std::string &id,
                              const std::string &returnType,
                              const std::shared_ptr<AST_BODY> &body) noexcept
      : AST(type), id_{id}, returnType_{returnType}, body_{body} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::string &returnType() const noexcept {
    return returnType_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }

  [[nodiscard]] virtual std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept = 0;

  [[nodiscard]] virtual std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept = 0;
};

} // namespace nicole

#endif