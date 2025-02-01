#ifndef AST_TERNARY_H
#define AST_TERNARY_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_TERNARY : public AST {
private:
  std::shared_ptr<AST> condition_;
  std::shared_ptr<AST> first_;
  std::shared_ptr<AST> second_;

public:
  explicit AST_TERNARY(const std::shared_ptr<AST> &condition,
                       const std::shared_ptr<AST> &first,
                       const std::shared_ptr<AST> &second) noexcept
      : AST(AST_TYPE::TERNARY), condition_{condition}, first_{first},
        second_{second} {}

  [[nodiscard]] const std::shared_ptr<AST> &condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::shared_ptr<AST> &first() const noexcept {
    return first_;
  }

  [[nodiscard]] const std::shared_ptr<AST> &second() const noexcept {
    return second_;
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