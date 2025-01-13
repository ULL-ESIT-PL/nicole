#ifndef AST_PTR_H
#define AST_PTR_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_PTR : public AST {
private:
  std::string id_;
  std::string type_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_PTR(const std::string &id, const std::string &type,
                   const std::shared_ptr<AST> &value) noexcept
      : AST(AST_TYPE::PTR), id_{id}, type_{type}, value_{value} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::string &type() const noexcept { return type_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }
};

} // namespace nicole

#endif