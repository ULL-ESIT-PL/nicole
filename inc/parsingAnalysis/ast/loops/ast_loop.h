#ifndef AST_LOOP_H
#define AST_LOOP_H

#include "../conditionals/ast_condition.h"
#include "../statements/ast_body.h"
#include <memory>

namespace nicole {

class AST_LOOP : public AST {
protected:
  std::shared_ptr<AST_CONDITION> condition_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_LOOP(const AST_TYPE type,
                    const std::shared_ptr<AST_CONDITION> &condition,
                    const std::shared_ptr<AST_BODY> &body) noexcept
      : AST(type), condition_{condition}, body_{body} {}

  [[nodiscard]] const std::shared_ptr<AST_CONDITION> &
  condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif // AST_LOOP_H
