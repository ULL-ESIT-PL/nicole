#ifndef AST_CASE_H
#define AST_CASE_H

#include "../ast.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_CASE : public AST {
private:
  std::string type_;
  std::vector<std::shared_ptr<AST>> values_;

public:
  explicit AST_CASE(const std::string &type,
                    const std::vector<std::shared_ptr<AST>> &values,
                    const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::CASE, sourceLocation), type_{type}, values_{values} {}

  [[nodiscard]] const std::string &type() const noexcept { return type_; }

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &
  values() const noexcept {
    return values_;
  }
};

} // namespace nicole

#endif