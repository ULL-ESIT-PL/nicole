#ifndef BUILDER_H
#define BUILDER_H

#include "ast/assignments/ast_assignment.h"
#include "ast/assignments/ast_selfAdd.h"
#include "ast/assignments/ast_selfDiv.h"
#include "ast/assignments/ast_selfMult.h"
#include "ast/assignments/ast_selfSub.h"

#include "ast/functions/ast_funcCall.h"
#include "ast/functions/ast_funcDecl.h"
#include "ast/functions/ast_return.h"

#include "ast/literals/ast_bool.h"
#include "ast/literals/ast_char.h"
#include "ast/literals/ast_double.h"
#include "ast/literals/ast_float.h"
#include "ast/literals/ast_int.h"
#include "ast/literals/ast_null.h"
#include "ast/literals/ast_string.h"
#include "ast/operators/binary/ast_add.h"
#include "ast/operators/binary/ast_and.h"
#include "ast/operators/binary/ast_bigger.h"
#include "ast/operators/binary/ast_biggerEqual.h"
#include "ast/operators/binary/ast_div.h"
#include "ast/operators/binary/ast_equal.h"
#include "ast/operators/binary/ast_module.h"
#include "ast/operators/binary/ast_mult.h"
#include "ast/operators/binary/ast_notEqual.h"
#include "ast/operators/binary/ast_or.h"
#include "ast/operators/binary/ast_smaller.h"
#include "ast/operators/binary/ast_smallerEqual.h"
#include "ast/operators/binary/ast_sub.h"

#include "ast/operators/unary/ast_decrement.h"
#include "ast/operators/unary/ast_increment.h"
#include "ast/operators/unary/ast_neg.h"
#include "ast/operators/unary/ast_not.h"

#include "ast/vector/ast_vector.h"

#include "ast/userTypes/ast_class.h"
#include "ast/userTypes/ast_enum.h"
#include "ast/userTypes/ast_struct.h"

#include "ast/utils/ast_import.h"
#include "ast/utils/ast_print.h"

#include "ast/statements/ast_body.h"
#include "ast/statements/ast_comma.h"
#include "ast/statements/ast_statement.h"

#include "ast/loops/ast_doWhile.h"
#include "ast/loops/ast_for.h"
#include "ast/loops/ast_pass.h"
#include "ast/loops/ast_stop.h"
#include "ast/loops/ast_while.h"

#include "ast/conditionals/ast_case.h"
#include "ast/conditionals/ast_if.h"
#include "ast/conditionals/ast_switch.h"
#include "ast/conditionals/ast_ternary.h"

#include "../errors.h"
#include "checkPosition.h"
#include <expected>

namespace nicole {

class Builder final {
private:
  Builder() noexcept = delete;

public:
  // Literals
  [[nodiscard]] static std::expected<std::shared_ptr<AST_BOOL>, Error>
  createBool(const bool value, const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_CHAR>, Error>
  createChar(const char value, const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DOUBLE>, Error>
  createDouble(const double value,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_FLOAT>, Error>
  createFloat(const float value, const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_INT>, Error>
  createInt(const int value, const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_NULL>, Error>
  createNull(const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_STRING>, Error>
  createString(const std::string value,
               const SourceLocation &sourceLocation) noexcept;

  // Vectors
  [[nodiscard]] static std::expected<std::shared_ptr<AST_VECTOR>, Error>
  createVector(const std::string type,
               const std::vector<std::shared_ptr<AST_VECTOR>> values,
               const SourceLocation &sourceLocation) noexcept;

  // Pointers

  // Binary
  [[nodiscard]] static std::expected<std::shared_ptr<AST_ADD>, Error>
  createAdd(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SUB>, Error>
  createSub(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_MULT>, Error>
  createMult(const Token &op, const std::shared_ptr<AST> &left,
             const std::shared_ptr<AST> &right,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DIV>, Error>
  createDiv(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_MODULE>, Error>
  createModule(const Token &op, const std::shared_ptr<AST> &left,
               const std::shared_ptr<AST> &right,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SMALLER>, Error>
  createSmaller(const Token &op, const std::shared_ptr<AST> &left,
                const std::shared_ptr<AST> &right,
                const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SMALLER_EQUAL>, Error>
  createSmallerEqual(const Token &op, const std::shared_ptr<AST> &left,
                     const std::shared_ptr<AST> &right,
                     const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_BIGGER>, Error>
  createBigger(const Token &op, const std::shared_ptr<AST> &left,
               const std::shared_ptr<AST> &right,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_BIGGER_EQUAL>, Error>
  createBiggerEqual(const Token &op, const std::shared_ptr<AST> &left,
                    const std::shared_ptr<AST> &right,
                    const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_EQUAL>, Error>
  createEqual(const Token &op, const std::shared_ptr<AST> &left,
              const std::shared_ptr<AST> &right,
              const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_NOT_EQUAL>, Error>
  createNotEqual(const Token &op, const std::shared_ptr<AST> &left,
                 const std::shared_ptr<AST> &right,
                 const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_OR>, Error>
  createOr(const Token &op, const std::shared_ptr<AST> &left,
           const std::shared_ptr<AST> &right,
           const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_AND>, Error>
  createAnd(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right,
            const SourceLocation &sourceLocation) noexcept;

  // Unary
  [[nodiscard]] static std::expected<std::shared_ptr<AST_NEG>, Error>
  createNeg(const Token &op, const std::shared_ptr<AST> &value,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_NOT>, Error>
  createNot(const Token &op, const std::shared_ptr<AST> &value,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_INCREMENT>, Error>
  createIncrement(const Token &op, const std::shared_ptr<AST> &value,
                  const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DECREMENT>, Error>
  createDecrement(const Token &op, const std::shared_ptr<AST> &value,
                  const SourceLocation &sourceLocation) noexcept;

  // Asignment
  [[nodiscard]] static std::expected<std::shared_ptr<AST_ASSIGNMENT>, Error>
  createAssignment(const std::string &id, const std::shared_ptr<AST> &value,
                   const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SELF_ADD>, Error>
  createSelfAdd(const std::string &id, const std::shared_ptr<AST> &value,
                const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SELF_SUB>, Error>
  createSelfSub(const std::string &id, const std::shared_ptr<AST> &value,
                const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SELF_MULT>, Error>
  createSelfMult(const std::string &id, const std::shared_ptr<AST> &value,
                 const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SELF_DIV>, Error>
  createSelfDiv(const std::string &id, const std::shared_ptr<AST> &value,
                const SourceLocation &sourceLocation) noexcept;

  // Utils
  [[nodiscard]] static std::expected<std::shared_ptr<AST_PRINT>, Error>
  createPrint(const std::shared_ptr<AST_COMMA> &values,
              const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_IMPORT>, Error>
  createImport(const std::filesystem::path &path,
               const SourceLocation &sourceLocation) noexcept;

  // Statements
  [[nodiscard]] static std::expected<std::shared_ptr<AST_STATEMENT>, Error>
  createStatement(const std::shared_ptr<AST> &expression,
                  const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_BODY>, Error>
  createBody(const std::vector<std::shared_ptr<AST_STATEMENT>> &body,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_COMMA>, Error>
  createCOMMA(const std::vector<std::shared_ptr<AST_STATEMENT>> &body,
              const SourceLocation &sourceLocation) noexcept;

  // Loops
  [[nodiscard]] static std::expected<std::shared_ptr<AST_WHILE>, Error>
  createWhile(const std::shared_ptr<AST> &condition,
              const std::shared_ptr<AST_BODY> &body,
              const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_FOR>, Error>
  createFor(const std::shared_ptr<AST_COMMA> &init,
            const std::shared_ptr<AST> &condition,
            const std::shared_ptr<AST_COMMA> &update,
            const std::shared_ptr<AST_BODY> &body,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DO_WHILE>, Error>
  createDoWhile(const std::shared_ptr<AST_BODY> &body,
                const std::shared_ptr<AST> &condition,
                const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_PASS>, Error>
  createPass(const std::shared_ptr<AST> &fatherLoop,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_STOP>, Error>
  createStop(const std::shared_ptr<AST> &fatherLoop,
             const SourceLocation &sourceLocation) noexcept;

  // Conditionals
  [[nodiscard]] static std::expected<std::shared_ptr<AST_IF>, Error>
  createIf(const std::shared_ptr<AST> &condition,
           const std::shared_ptr<AST_BODY> &body,
           const std::vector<std::shared_ptr<AST_ELSE_IF>> &elseIf,
           const std::shared_ptr<AST_BODY> &elseBody,
           const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_IF>, Error>
  createElseIf(const std::shared_ptr<AST> &condition,
           const std::shared_ptr<AST_BODY> &body,
           const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SWITCH>, Error>
  createSwitch(const std::shared_ptr<AST> &condition,
               const std::vector<std::shared_ptr<AST_CASE>> &cases,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_CASE>, Error>
  createCase(const std::shared_ptr<AST> &match,
             const std::shared_ptr<AST_BODY> &body,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_TERNARY>, Error>
  createTernary(const std::shared_ptr<AST> &condition,
                const std::shared_ptr<AST> &first,
                const std::shared_ptr<AST> &second,
                const SourceLocation &sourceLocation) noexcept;

  // Functions
  [[nodiscard]] static std::expected<std::shared_ptr<AST_FUNC_CALL>, Error>
  createFunCall(const std::string &id,
                const std::vector<std::shared_ptr<AST>> &parameters,
                const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
  createFuncDecl(const std::string &id, const Parameters &params,
                 const std::string &returnType,
                 const std::shared_ptr<AST_BODY> &body,
                 const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_RETURN>, Error>
  createReturn(const std::shared_ptr<AST> &value,
               const SourceLocation &sourceLocation) noexcept;

  // Usert types
  [[nodiscard]] static std::expected<std::shared_ptr<AST_ENUM>, Error>
  createEnum(const std::vector<std::string> &enumIdentifiers,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_STRUCT>, Error>
  createStruct(const std::string &id, const Attributes &attributes,
               const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods,
               const std::shared_ptr<AST_FUNC_DECL> &constructor,
               const std::shared_ptr<AST_FUNC_DECL> &destructor,
               const std::shared_ptr<AST_FUNC_DECL> &addOverloading,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_CLASS>, Error>
  createClass(const std::string &id, const Attributes &attributes,
              const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods,
              const std::shared_ptr<AST_FUNC_DECL> &constructor,
              const std::shared_ptr<AST_FUNC_DECL> &destructor,
              const std::shared_ptr<AST_FUNC_DECL> &addOverloading,
              const SourceLocation &sourceLocation) noexcept;

  // Chained expression
};

} // namespace nicole

#endif