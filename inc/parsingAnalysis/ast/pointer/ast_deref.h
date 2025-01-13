#ifndef AST_DEREF_H
#define AST_DEREF_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_DEREF : public AST {
private:
  std::shared_ptr<AST> value_;

public:
  explicit AST_DEREF(const std::shared_ptr<AST> &value) noexcept
      : AST(AST_TYPE::PTR_DEREFENCE), value_{value} {}

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }
};

} // namespace nicole

#endif