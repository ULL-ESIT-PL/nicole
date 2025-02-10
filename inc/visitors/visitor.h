#ifndef VISITOR_H
#define VISITOR_H

#include "../errors.h"
#include <expected>

namespace nicole {

// Forward declaration to avoid circular dependency
enum class TokenType;

class AST;

class AST_BOOL;
class AST_CHAR;
class AST_DOUBLE;
class AST_FLOAT;
class AST_INT;
class AST_NULL;
class AST_STRING;

class AST_VECTOR;
class AST_INDEX;

class AST_DELETE;
class AST_NEW;
class AST_DEREF;
class AST_PTR;

class AST_BINARY;

class AST_UNARY;

class AST_ASSIGNMENT;

class AST_PRINT;
class AST_IMPORT;

class AST_STATEMENT;
class AST_BODY;

class AST_WHILE;
class AST_FOR;
class AST_DO_WHILE;
class AST_PASS;
class AST_STOP;

class AST_IF;
class AST_ELSE_IF;
class AST_SWITCH;
class AST_CASE;
class AST_DEFAULT;
class AST_TERNARY;
class AST_CONDITION;

class AST_FUNC_CALL;
class AST_FUNC_DECL;
class AST_RETURN;

class AST_ENUM;
class AST_STRUCT;
class AST_CLASS;
class AST_ATTR_ACCESS;
class AST_METHOD_CALL;
class AST_THIS;
class AST_CONSTRUCTOR_CALL;

class AST_AUTO_DECL;
class AST_VAR_TYPED_DECL;
class AST_VAR_CALL;

class AST_CHAINED;

class Tree;

// Abstract class that represents the visitor pattern
template <class T> class Visitor {
public:
  virtual ~Visitor() = default;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_BOOL *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_CHAR *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_DOUBLE *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_FLOAT *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_INT *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_NULL *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_STRING *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_VECTOR *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_INDEX *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_DELETE *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_NEW *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_DEREF *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_PTR *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_BINARY *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_UNARY *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_ASSIGNMENT *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_PRINT *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_IMPORT *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_STATEMENT *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_BODY *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_WHILE *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_FOR *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_DO_WHILE *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_PASS *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_STOP *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_IF *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_ELSE_IF *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_SWITCH *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_CASE *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_DEFAULT *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_TERNARY *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_CONDITION *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_FUNC_CALL *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_FUNC_DECL *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_RETURN *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_ENUM *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_STRUCT *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_CLASS *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_ATTR_ACCESS *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_METHOD_CALL *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_THIS *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_CONSTRUCTOR_CALL *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_AUTO_DECL *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_VAR_TYPED_DECL *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_VAR_CALL *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const AST_CHAINED *node) const noexcept = 0;

  [[nodiscard]] virtual std::expected<T, Error>
  visit(const Tree *tree) const noexcept = 0;
};

} // namespace nicole

#endif