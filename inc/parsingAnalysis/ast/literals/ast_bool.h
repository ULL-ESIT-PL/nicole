#ifndef AST_BOOL_H
#define AST_BOOL_H

#include "../ast.h"

namespace nicole {

class AST_BOOL final : public AST {
private:
  bool value_;

public:
  explicit AST_BOOL(const bool value, const SourceLocation &sourceLocation,
                    const std::shared_ptr<AST> &father = nullptr) noexcept
      : AST{AST_TYPE::BOOL, sourceLocation, father}, value_{value} {}

  ~AST_BOOL() noexcept = default;

  [[nodiscard]] bool value() const noexcept { return value_; }
};

} // namespace nicole

#endif // AST_H
