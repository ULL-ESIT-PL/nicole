#ifndef AST_ASSIGNMENT_H
#define AST_ASSIGNMENT_H

#include "../ast.h"
#include <string>

namespace nicole {

class AST_ASSIGNMENT final : public AST {
private:
  std::string id_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_ASSIGNMENT(const std::string &id,
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