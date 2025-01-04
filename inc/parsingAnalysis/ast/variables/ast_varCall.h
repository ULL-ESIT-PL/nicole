#ifndef AST_ASSIGNMENT_H
#define AST_ASSIGNMENT_H

#include "../ast.h"
#include <string>

namespace nicole {

class AST_ASSIGNMENT final : public AST {
private:
  std::string id_;

public:
  explicit AST_ASSIGNMENT(const std::string &id,
                          const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::CASE, sourceLocation), id_{id} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }
};

} // namespace nicole

#endif