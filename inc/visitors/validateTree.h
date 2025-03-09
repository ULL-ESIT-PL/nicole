#ifndef VALIDATE_TREE_H
#define VALIDATE_TREE_H

#include "visitor.h"

class CheckPosition;

namespace nicole {

class ValidateTree final : public Visitor<bool> {
public:
  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_BOOL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_CHAR *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_DOUBLE *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_FLOAT *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_INT *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_NULL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_STRING *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_VECTOR *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_INDEX *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_DELETE *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_NEW *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_DEREF *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_BINARY *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_UNARY *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_ASSIGNMENT *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_PRINT *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_IMPORT *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_STATEMENT *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_BODY *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_WHILE *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_FOR *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_DO_WHILE *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_PASS *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_STOP *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_IF *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_ELSE_IF *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_SWITCH *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_CASE *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_DEFAULT *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_TERNARY *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_CONDITION *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_FUNC_CALL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_FUNC_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_RETURN *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_PARAMETER *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_ENUM *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_ENUM_ACCESS *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_STRUCT *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_ATTR_ACCESS *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_METHOD_CALL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_METHOD_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_CONSTRUCTOR_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_SUPER *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_DESTRUCTOR_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_THIS *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_CONSTRUCTOR_CALL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_AUTO_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_VAR_TYPED_DECL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_VAR_CALL *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const AST_CHAINED *node) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  visit(const Tree *tree) const noexcept override;

  [[nodiscard]] std::expected<bool, Error>
  validate(const Tree *tree) const noexcept {
    return visit(tree);
  }
};

} // namespace nicole

#endif