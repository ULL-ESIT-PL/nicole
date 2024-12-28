#ifndef AST_NULL_H
#define AST_NULL_H

#include "../ast.h"

namespace nicole {

class AST_NULL final : public AST {
public:
  explicit AST_NULL(const std::shared_ptr<AST> &father = nullptr) noexcept
      : AST{AST_TYPE::NULLPTR, father} {}

  ~AST_NULL() noexcept = default;
};

} // namespace nicole

#endif // AST_H
