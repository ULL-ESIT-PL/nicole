#ifndef AST_CONST_DECL_H
#define AST_CONST_DECL_H

#include "../ast.h"

namespace nicole {

class AST_CONST_DECL : public AST {
private:
  std::string id_;
  std::string type_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_CONST_DECL(const std::string &id, const std::string &type,
                        const std::shared_ptr<AST> &value,
                        const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::CONST_DECL, sourceLocation), id_{id}, type_{type},
        value_{value} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::string &type() const noexcept { return type_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }
};

} // namespace nicole

#endif