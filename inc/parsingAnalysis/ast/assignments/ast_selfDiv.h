#ifndef AST_SELF_DIV_H
#define AST_SELF_DIV_H

#include "../ast.h"
#include <string>

namespace nicole {

class AST_SELF_DIV final : public AST {
private:
  std::string id_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_SELF_DIV(const std::string &id,
                          const std::shared_ptr<AST> &value,
                          const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::CASE, sourceLocation), id_{id}, value_{value} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }
};

} // namespace nicole

#endif