#ifndef AST_ASSIGNMENT_H
#define AST_ASSIGNMENT_H

#include "../ast.h"
#include <string>
#include <vector>

namespace nicole {

class AST_ASSIGNMENT final : public AST {
private:
  std::vector<std::string> enumIdentifiers_;

public:
  explicit AST_ASSIGNMENT(const std::vector<std::string> &enumIdentifiers,
                          const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::CASE, sourceLocation), enumIdentifiers_{enumIdentifiers} {
  }

  [[nodiscard]] const std::vector<std::string> &identifiers() const noexcept {
    return enumIdentifiers_;
  }
};

} // namespace nicole

#endif