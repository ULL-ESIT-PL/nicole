#ifndef AST_CHAR_H
#define AST_CHAR_H

#include "../ast.h"

namespace nicole {

class AST_CHAR final : public AST {
private:
  char value_;

public:
  explicit AST_CHAR(const char value, const SourceLocation &sourceLocation,
                    const std::shared_ptr<AST> &father = nullptr) noexcept
      : AST{AST_TYPE::CHAR, sourceLocation, father}, value_{value} {}

  ~AST_CHAR() noexcept = default;

  [[nodiscard]] char value() const noexcept { return value_; }
};

} // namespace nicole

#endif // AST_H
