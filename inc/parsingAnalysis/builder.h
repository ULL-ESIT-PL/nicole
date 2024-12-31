#ifndef BUILDER_H
#define BUILDER_H

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
#include "ast/operators/unary/ast_not.h"

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

namespace nicole {

class Builder final {
private:
  Builder() noexcept = delete;

public:
  [[nodiscard]] static std::shared_ptr<AST_BOOL>
  createBool(const bool value, const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_CHAR>
  createChar(const char value, const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_DOUBLE>
  createDouble(const double value,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_FLOAT>
  createFloat(const float value, const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_INT>
  createInt(const int value, const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_NULL>
  createNull(const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_STRING>
  createString(const std::string value,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_ADD>
  createAdd(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_SUB>
  createSub(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_MULT>
  createMult(const Token &op, const std::shared_ptr<AST> &left,
             const std::shared_ptr<AST> &right,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_DIV>
  createDiv(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_MODULE>
  createModule(const Token &op, const std::shared_ptr<AST> &left,
               const std::shared_ptr<AST> &right,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_SMALLER>
  createSmaller(const Token &op, const std::shared_ptr<AST> &left,
                const std::shared_ptr<AST> &right,
                const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_SMALLER_EQUAL>
  createSmallerEqual(const Token &op, const std::shared_ptr<AST> &left,
                     const std::shared_ptr<AST> &right,
                     const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_BIGGER>
  createBigger(const Token &op, const std::shared_ptr<AST> &left,
               const std::shared_ptr<AST> &right,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_BIGGER_EQUAL>
  createBiggerEqual(const Token &op, const std::shared_ptr<AST> &left,
                    const std::shared_ptr<AST> &right,
                    const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_EQUAL>
  createEqual(const Token &op, const std::shared_ptr<AST> &left,
              const std::shared_ptr<AST> &right,
              const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_NOT_EQUAL>
  createNotEqual(const Token &op, const std::shared_ptr<AST> &left,
                 const std::shared_ptr<AST> &right,
                 const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_OR>
  createOr(const Token &op, const std::shared_ptr<AST> &left,
           const std::shared_ptr<AST> &right,
           const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_AND>
  createAnd(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_NOT>
  createNot(const Token &op, const std::shared_ptr<AST> &value,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_INCREMENT>
  createIncrement(const Token &op, const std::shared_ptr<AST> &value,
                  const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_DECREMENT>
  createDecrement(const Token &op, const std::shared_ptr<AST> &value,
                  const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_PRINT>
  createPrint(const std::shared_ptr<AST> &value,
              const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_IMPORT>
  createImport(const std::filesystem::path &path,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_STATEMENT>
  createStatement(const std::shared_ptr<AST> &expression,
                  const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_BODY>
  createBody(const std::vector<std::shared_ptr<AST_STATEMENT>> &body,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_COMMA>
  createCOMMA(const std::vector<std::shared_ptr<AST_STATEMENT>> &body,
              const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_WHILE>
  createWhile(const std::shared_ptr<AST_BODY> &body,
              const std::shared_ptr<AST> &condition,
              const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_FOR>
  createFor(const std::shared_ptr<AST_COMMA> &init,
            const std::shared_ptr<AST> &condition,
            const std::shared_ptr<AST_COMMA> &update,
            const std::shared_ptr<AST_BODY> &body,
            const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_DO_WHILE>
  createDoWhile(const std::shared_ptr<AST> &condition,
                const std::shared_ptr<AST_BODY> &body,
                const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_PASS>
  createPass(const std::shared_ptr<AST> &fatherLoop,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_STOP>
  createStop(const std::shared_ptr<AST> &fatherLoop,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_IF>
  createIf(const std::shared_ptr<AST> &condition,
           const std::shared_ptr<AST_BODY> &body,
           const std::vector<std::shared_ptr<AST_IF>> &elseIf,
           const std::shared_ptr<AST_BODY> &elseBody,
           const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_SWITCH>
  createSwitch(const std::shared_ptr<AST> &condition,
               const std::vector<std::shared_ptr<AST_CASE>> &cases,
               const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_CASE>
  createCase(const std::shared_ptr<AST> &match,
             const std::shared_ptr<AST_BODY> &body,
             const SourceLocation &sourceLocation) noexcept;

  [[nodiscard]] static std::shared_ptr<AST_TERNARY>
  createTernary(const std::shared_ptr<AST> &condition,
                const std::shared_ptr<AST> &first,
                const std::shared_ptr<AST> &second,
                const SourceLocation &sourceLocation) noexcept;
};

} // namespace nicole

#endif