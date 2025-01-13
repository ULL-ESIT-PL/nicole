#ifndef VISITOR_H
#define VISITOR_H

#include "../parsingAnalysis/builder.h"

namespace nicole {

// Forward declaration to avoid circular dependency
/*
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

class AST_DELETE;
class AST_NEW;
class AST_DEREF;
class AST_PTR;

class AST_ADD;
class AST_SUB;
class AST_MULT;
class AST_DIV;
class AST_MODULE;
class AST_SMALLER;
class AST_SMALLER_EQUAL;
class AST_BIGGER;
class AST_BIGGER_EQUAL;
class AST_EQUAL;
class AST_NOT_EQUAL;
class AST_OR;
class AST_AND;

class AST_NEG;
class AST_NOT;
class AST_INCREMENT;
class AST_DECREMENT;

class AST_ASSIGNMENT;
class AST_SELF_ADD;
class AST_SELF_SUB;
class AST_SELF_MULT;
class AST_SELF_DIV;

class AST_PRINT;
class AST_IMPORT;

class AST_STATEMENT;
class AST_BODY;
class AST_COMMA;

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

class AST_FUNC_CALL;
class AST_FUNC_DECL;
class AST_RETURN;

class AST_ENUM;
class AST_STRUCT;
class AST_CLASS;

class AST_AUTO_DECL;
class AST_LET_DECL;
class AST_CONST_DECL;

class AST_CHAINED;

class Tree;
*/
// Abstract class that represents the visitor pattern
template <class T> class Visitor {
public:
  virtual ~Visitor() = default;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_BOOL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_CHAR> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_DOUBLE> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_FLOAT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_INT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_NULL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_STRING> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_VECTOR> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_DELETE> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_NEW> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_DEREF> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_PTR> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_ADD> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_SUB> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_MULT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_DIV> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_MODULE> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_SMALLER> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_SMALLER_EQUAL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_BIGGER> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_BIGGER_EQUAL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_EQUAL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_NOT_EQUAL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_OR> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_AND> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_NEG> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_NOT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_DECREMENT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_INCREMENT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_ASSIGNMENT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_SELF_ADD> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_SELF_SUB> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_SELF_MULT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_SELF_DIV> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_PRINT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_IMPORT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_STATEMENT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_BODY> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_COMMA> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_WHILE> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_FOR> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_DO_WHILE> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_PASS> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_STOP> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_IF> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_ELSE_IF> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_SWITCH> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_CASE> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_DEFAULT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_TERNARY> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_FUNC_CALL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_FUNC_DECL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_RETURN> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_ENUM> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_STRUCT> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_CLASS> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_AUTO_DECL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_LET_DECL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_CONST_DECL> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<AST_CHAINED> &node) const noexcept = 0;

  virtual std::expected<T, Error>
  visit(const std::shared_ptr<Tree> &tree) const noexcept = 0;
};

} // namespace nicole

#endif