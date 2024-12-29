#ifndef AST_DOUBLE_H
#define AST_DOUBLE_H

#include "../ast.h"

namespace nicole {

class AST_DOUBLE final : public AST {
private:
  double value_;

public:
  explicit AST_DOUBLE(const double value, const SourceLocation &sourceLocation,
                      const std::shared_ptr<AST> &father = nullptr) noexcept
      : AST{AST_TYPE::DOUBLE, sourceLocation, father}, value_{value} {}

  ~AST_DOUBLE() noexcept = default;

  [[nodiscard]] double value() const noexcept { return value_; }
};

} // namespace nicole

#endif // AST_H
