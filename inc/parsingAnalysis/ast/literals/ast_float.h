#ifndef AST_FLOAT_H
#define AST_FLOAT_H

#include "../ast.h"

namespace nicole {

class AST_FLOAT final : public AST {
private:
  float value_;

public:
  explicit AST_FLOAT(const float value,
                     const std::shared_ptr<AST> &father = nullptr) noexcept
      : AST{AST_TYPE::FLOAT, father}, value_{value} {}

  ~AST_FLOAT() noexcept = default;

  [[nodiscard]] float value() const noexcept { return value_; }
};

} // namespace nicole

#endif // AST_H
