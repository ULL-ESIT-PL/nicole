#ifndef AST_CHAR_H
#define AST_CHAR_H

#include "../ast.h"

namespace nicole {

class AST_CHAR final : public AST {
private:
  char value_;

public:
  explicit AST_CHAR(const char value,
                    const SourceLocation &sourceLocation) noexcept
      : AST{AST_TYPE::CHAR, sourceLocation}, value_{value} {}

  ~AST_CHAR() noexcept = default;

  [[nodiscard]] char value() const noexcept { return value_; }
};

} // namespace nicole

#endif // AST_H
