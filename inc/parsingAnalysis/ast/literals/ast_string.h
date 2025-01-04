#ifndef AST_STRING_H
#define AST_STRING_H

#include "../ast.h"

namespace nicole {

class AST_STRING final : public AST {
private:
  std::string value_;

public:
  explicit AST_STRING(const std::string value,
                      const SourceLocation &sourceLocation) noexcept
      : AST{AST_TYPE::STRING, sourceLocation}, value_{value} {}

  ~AST_STRING() noexcept = default;

  [[nodiscard]] const std::string &value() const noexcept { return value_; }
};

} // namespace nicole

#endif // AST_H
