#ifndef AST_ENUM_H
#define AST_ENUM_H

#include "../ast.h"
#include <string>
#include <vector>

namespace nicole {

class AST_ENUM final : public AST {
private:
  std::vector<std::string> enumIdentifiers_;

public:
  explicit AST_ENUM(const std::vector<std::string> &enumIdentifiers,
                    const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::ENUM, sourceLocation), enumIdentifiers_{enumIdentifiers} {
  }

  [[nodiscard]] const std::vector<std::string> &identifiers() const noexcept {
    return enumIdentifiers_;
  }
};

} // namespace nicole

#endif