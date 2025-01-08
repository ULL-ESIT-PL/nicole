#ifndef AST_VECTOR_H
#define AST_VECTOR_H

#include "../ast.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_VECTOR : public AST {
private:
  std::string type_;
  std::vector<std::shared_ptr<AST>> values_;

public:
  explicit AST_VECTOR(const std::string &type,
                      const std::vector<std::shared_ptr<AST>> &values) noexcept
      : AST(AST_TYPE::VECTOR), type_{type}, values_{values} {}

  [[nodiscard]] const std::string &type() const noexcept { return type_; }

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &
  values() const noexcept {
    return values_;
  }
};

} // namespace nicole

#endif