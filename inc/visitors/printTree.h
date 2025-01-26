#ifndef PRINT_TREE_H
#define PRINT_TREE_H

#include "visitor.h"

namespace nicole {

class PrintTree final : public Visitor<std::string> {
private:
  mutable std::string indent_{};

  void increaseIndent() const noexcept { indent_ += "  "; }

  void decreaseIndent() const noexcept {
    if (indent_.size() >= 2)
      indent_ = indent_.substr(0, indent_.size() - 2);
  }

public:
  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_BOOL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_CHAR *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_DOUBLE *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_FLOAT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_INT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_NULL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_STRING *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_VECTOR *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_INDEX *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_DELETE *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_NEW *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_DEREF *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_PTR *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_ADD *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_SUB *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_MULT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_DIV *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_MODULE *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_SMALLER *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_SMALLER_EQUAL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_BIGGER *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_BIGGER_EQUAL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_EQUAL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_NOT_EQUAL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_OR *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_AND *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_NEG *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_NOT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_DECREMENT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_INCREMENT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_ASSIGNMENT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_SELF_ADD *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_SELF_SUB *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_SELF_MULT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_SELF_DIV *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_PRINT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_IMPORT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_STATEMENT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_BODY *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_COMMA *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_WHILE *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_FOR *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_DO_WHILE *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_PASS *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_STOP *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_IF *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_ELSE_IF *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_SWITCH *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_CASE *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_DEFAULT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_TERNARY *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_FUNC_CALL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_FUNC_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_RETURN *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_ENUM *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_STRUCT *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_CLASS *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_ATTR_ACCESS *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_METHOD_CALL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_AUTO_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_LET_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_CONST_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_VAR_CALL *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const AST_CHAINED *node) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  visit(const Tree *tree) const noexcept override;

  [[nodiscard]] std::expected<std::string, Error>
  print(const Tree *tree) const noexcept { return visit(tree); }
};

} // namespace nicole

#endif