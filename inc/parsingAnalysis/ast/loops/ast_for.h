#ifndef AST_FOR_H
#define AST_FOR_H

#include "../statements/ast_body.h"
#include "../statements/ast_comma.h"
#include <memory>

namespace nicole {

class AST_FOR : public AST {
private:
  std::vector<std::shared_ptr<AST>> init_;
  std::shared_ptr<AST> condition_;
  std::vector<std::shared_ptr<AST>> update_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_FOR(const std::vector<std::shared_ptr<AST>> &init,
                   const std::shared_ptr<AST> &condition,
                   const std::vector<std::shared_ptr<AST>> &update,
                   const std::shared_ptr<AST_BODY> &body) noexcept
      : AST(AST_TYPE::FOR), init_{init}, condition_{condition}, update_{update},
        body_{body} {}

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &init() const noexcept {
    return init_;
  }

  [[nodiscard]] const std::shared_ptr<AST> &condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &
  update() const noexcept {
    return update_;
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