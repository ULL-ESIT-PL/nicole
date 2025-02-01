#ifndef AST_DO_WHILE_H
#define AST_DO_WHILE_H

#include "../statements/ast_body.h"
#include <memory>

namespace nicole {

class AST_DO_WHILE : public AST {
private:
  std::shared_ptr<AST_BODY> body_;
  std::shared_ptr<AST> condition_;

public:
  explicit AST_DO_WHILE(const std::shared_ptr<AST_BODY> &body,
                        const std::shared_ptr<AST> &condition) noexcept
      : AST(AST_TYPE::DO_WHILE), body_{body}, condition_{condition} {}

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }

  [[nodiscard]] const std::shared_ptr<AST> &condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif