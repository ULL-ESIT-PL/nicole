#ifndef AST_INDEX_H
#define AST_INDEX_H

#include "../ast.h"

namespace nicole {

class AST_INDEX final : public AST {
private:
  std::shared_ptr<AST> index_;

public:
  explicit AST_INDEX(const std::shared_ptr<AST> &index) noexcept
      : AST{AST_TYPE::INDEX}, index_{index} {}

  ~AST_INDEX() noexcept = default;

  [[nodiscard]] const std::shared_ptr<AST> &index() const noexcept {
    return index_;
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

#endif // AST_H
