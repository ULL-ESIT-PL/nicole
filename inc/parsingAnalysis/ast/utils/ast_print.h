#ifndef AST_PRINT_H
#define AST_PRINT_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_PRINT : public AST {
private:
  std::shared_ptr<AST> value_;

public:
  explicit AST_PRINT(const std::shared_ptr<AST> &value,
                     const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::PRINT, sourceLocation), value_{value} {}

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }
};

} // namespace nicole

#endif