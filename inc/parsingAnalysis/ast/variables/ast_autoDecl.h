#ifndef AST_AUTO_DECL_H
#define AST_AUTO_DECL_H

#include "../ast.h"

namespace nicole {

class AST_AUTO_DECL : public AST {
private:
  std::string id_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_AUTO_DECL(const std::string &id,
                        const std::shared_ptr<AST> &value,
                        const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::AUTO_DECL, sourceLocation), id_{id}, value_{value} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }
};

} // namespace nicole

#endif