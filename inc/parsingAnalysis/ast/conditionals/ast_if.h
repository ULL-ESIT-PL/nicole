#ifndef AST_IF_H
#define AST_IF_H

#include "ast_elseIf.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_IF : public AST {
private:
  std::shared_ptr<AST> condition_;
  std::shared_ptr<AST_BODY> body_;
  std::vector<std::shared_ptr<AST_ELSE_IF>> elseIf_;
  std::shared_ptr<AST_BODY> elseBody_;

public:
  explicit AST_IF(const std::shared_ptr<AST> &condition,
                  const std::shared_ptr<AST_BODY> &body,
                  const std::vector<std::shared_ptr<AST_ELSE_IF>> &elseIf,
                  const std::shared_ptr<AST_BODY> &elseBody) noexcept
      : AST(AST_TYPE::IF), condition_{condition}, body_{body}, elseIf_{elseIf},
        elseBody_{elseBody} {}

  [[nodiscard]] const std::shared_ptr<AST> &condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST_ELSE_IF>> &
  elseIf() const noexcept {
    return elseIf_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &elseBody() const noexcept {
    return elseBody_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif