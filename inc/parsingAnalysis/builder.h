#ifndef BUILDER_H
#define BUILDER_H

#include "ast/assignments/ast_assignment.h"
#include "ast/assignments/ast_selfAdd.h"
#include "ast/assignments/ast_selfDiv.h"
#include "ast/assignments/ast_selfMult.h"
#include "ast/assignments/ast_selfSub.h"

#include "ast/conditionals/ast_default.h"
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

#include "ast/userTypes/ast_constructorCall.h"
#include "ast/userTypes/ast_this.h"
#include "ast/vector/ast_index.h"
#include "ast/vector/ast_vector.h"

#include "ast/userTypes/ast_attrAccess.h"
#include "ast/userTypes/ast_class.h"
#include "ast/userTypes/ast_enum.h"
#include "ast/userTypes/ast_methodCall.h"
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

#include "ast/variables/ast_autoDecl.h"
#include "ast/variables/ast_constDecl.h"
#include "ast/variables/ast_letDecl.h"
#include "ast/variables/ast_varCall.h"

#include "ast/pointer/ast_delete.h"
#include "ast/pointer/ast_deref.h"
#include "ast/pointer/ast_new.h"
#include "ast/pointer/ast_ptr.h"

#include "ast/chained/ast_chained.h"

#include "ast/tree.h"

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
  createBool(const bool value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_CHAR>, Error>
  createChar(const std::string &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DOUBLE>, Error>
  createDouble(const double value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_FLOAT>, Error>
  createFloat(const float value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_INT>, Error>
  createInt(const int value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_NULL>, Error>
  createNull() noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_STRING>, Error>
  createString(const std::string value) noexcept;

  // Vectors
  [[nodiscard]] static std::expected<std::shared_ptr<AST_VECTOR>, Error>
  createVector(const std::vector<std::shared_ptr<AST>> values) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_INDEX>, Error>
  createIndex(const std::shared_ptr<AST> value) noexcept;

  // Pointers
  [[nodiscard]] static std::expected<std::shared_ptr<AST_DELETE>, Error>
  createDelete(const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_NEW>, Error>
  createNew(const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DEREF>, Error>
  createDeref(const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_PTR>, Error>
  createPtr(const std::string &id, const std::string &type,
            const std::shared_ptr<AST> &value) noexcept;

  // Binary
  [[nodiscard]] static std::expected<std::shared_ptr<AST_ADD>, Error>
  createAdd(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SUB>, Error>
  createSub(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_MULT>, Error>
  createMult(const Token &op, const std::shared_ptr<AST> &left,
             const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DIV>, Error>
  createDiv(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_MODULE>, Error>
  createModule(const Token &op, const std::shared_ptr<AST> &left,
               const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SMALLER>, Error>
  createSmaller(const Token &op, const std::shared_ptr<AST> &left,
                const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SMALLER_EQUAL>, Error>
  createSmallerEqual(const Token &op, const std::shared_ptr<AST> &left,
                     const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_BIGGER>, Error>
  createBigger(const Token &op, const std::shared_ptr<AST> &left,
               const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_BIGGER_EQUAL>, Error>
  createBiggerEqual(const Token &op, const std::shared_ptr<AST> &left,
                    const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_EQUAL>, Error>
  createEqual(const Token &op, const std::shared_ptr<AST> &left,
              const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_NOT_EQUAL>, Error>
  createNotEqual(const Token &op, const std::shared_ptr<AST> &left,
                 const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_OR>, Error>
  createOr(const Token &op, const std::shared_ptr<AST> &left,
           const std::shared_ptr<AST> &right) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_AND>, Error>
  createAnd(const Token &op, const std::shared_ptr<AST> &left,
            const std::shared_ptr<AST> &right) noexcept;

  // Unary
  [[nodiscard]] static std::expected<std::shared_ptr<AST_NEG>, Error>
  createNeg(const Token &op, const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_NOT>, Error>
  createNot(const Token &op, const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_INCREMENT>, Error>
  createIncrement(const Token &op, const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DECREMENT>, Error>
  createDecrement(const Token &op, const std::shared_ptr<AST> &value) noexcept;

  // Asignment
  [[nodiscard]] static std::expected<std::shared_ptr<AST_ASSIGNMENT>, Error>
  createAssignment(const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SELF_ADD>, Error>
  createSelfAdd(const std::shared_ptr<AST> &left,
                const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SELF_SUB>, Error>
  createSelfSub(const std::shared_ptr<AST> &left,
                const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SELF_MULT>, Error>
  createSelfMult(const std::shared_ptr<AST> &left,
                 const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SELF_DIV>, Error>
  createSelfDiv(const std::shared_ptr<AST> &left,
                const std::shared_ptr<AST> &value) noexcept;

  // Utils
  [[nodiscard]] static std::expected<std::shared_ptr<AST_PRINT>, Error>
  createPrint(const std::vector<std::shared_ptr<AST>> &values) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_IMPORT>, Error>
  createImport(const std::filesystem::path &path) noexcept;

  // Statements
  [[nodiscard]] static std::expected<std::shared_ptr<AST_STATEMENT>, Error>
  createStatement(const std::shared_ptr<AST> &expression) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_BODY>, Error>
  createBody(const std::vector<std::shared_ptr<AST_STATEMENT>> &body) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_COMMA>, Error>
  createComma(const std::vector<std::shared_ptr<AST_STATEMENT>> &body) noexcept;

  // Loops
  [[nodiscard]] static std::expected<std::shared_ptr<AST_WHILE>, Error>
  createWhile(const std::shared_ptr<AST> &condition,
              const std::shared_ptr<AST_BODY> &body) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_FOR>, Error>
  createFor(const std::vector<std::shared_ptr<AST>> &init,
            const std::shared_ptr<AST> &condition,
            const std::vector<std::shared_ptr<AST>> &update,
            const std::shared_ptr<AST_BODY> &body) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DO_WHILE>, Error>
  createDoWhile(const std::shared_ptr<AST_BODY> &body,
                const std::shared_ptr<AST> &condition) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_PASS>, Error>
  createPass(const std::shared_ptr<AST> &fatherLoop) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_STOP>, Error>
  createStop(const std::shared_ptr<AST> &fatherLoop) noexcept;

  // Conditionals
  [[nodiscard]] static std::expected<std::shared_ptr<AST_IF>, Error>
  createIf(const std::shared_ptr<AST> &condition,
           const std::shared_ptr<AST_BODY> &body,
           const std::vector<std::shared_ptr<AST_ELSE_IF>> &elseIf,
           const std::shared_ptr<AST_BODY> &elseBody) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_ELSE_IF>, Error>
  createElseIf(const std::shared_ptr<AST> &condition,
               const std::shared_ptr<AST_BODY> &body) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_SWITCH>, Error>
  createSwitch(const std::shared_ptr<AST> &condition,
               const std::vector<std::shared_ptr<AST_CASE>> &cases,
               const std::shared_ptr<AST_DEFAULT> &default_) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_CASE>, Error>
  createCase(const std::shared_ptr<AST> &match,
             const std::shared_ptr<AST_BODY> &body) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_DEFAULT>, Error>
  createDefault(const std::shared_ptr<AST_BODY> &body) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_TERNARY>, Error>
  createTernary(const std::shared_ptr<AST> &condition,
                const std::shared_ptr<AST> &first,
                const std::shared_ptr<AST> &second) noexcept;

  // Functions
  [[nodiscard]] static std::expected<std::shared_ptr<AST_FUNC_CALL>, Error>
  createFunCall(const std::string &id,
                const std::vector<std::shared_ptr<AST>> &parameters) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
  createFuncDecl(const std::string &id, const Parameters &params,
                 const std::string &returnType,
                 const std::shared_ptr<AST_BODY> &body) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_RETURN>, Error>
  createReturn(const std::shared_ptr<AST> &value) noexcept;

  // Usert types
  [[nodiscard]] static std::expected<std::shared_ptr<AST_ENUM>, Error>
  createEnum(const std::string &id,
             const std::vector<std::string> &enumIdentifiers) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_STRUCT>, Error>
  createStruct(const std::string &id, std::unique_ptr<std::string> fatherType,
               const Attributes &attributes,
               const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods,
               const std::shared_ptr<AST_FUNC_DECL> &constructor,
               const std::shared_ptr<AST_FUNC_DECL> &destructor) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_CLASS>, Error>
  createClass(const std::string &id, std::unique_ptr<std::string> fatherType,
              const Attributes &attributes,
              const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods,
              const std::shared_ptr<AST_FUNC_DECL> &constructor,
              const std::shared_ptr<AST_FUNC_DECL> &destructor) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_ATTR_ACCESS>, Error>
  createAttrAccess(const std::string &id) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_METHOD_CALL>, Error>
  createMethodCall(
      const std::string &id,
      const std::vector<std::shared_ptr<AST>> &parameters) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_THIS>, Error>
  createThis() noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_CONSTRUCTOR_CALL>,
                                     Error>
  createConstructorCall(
      const std::string &id,
      const std::vector<std::shared_ptr<AST>> &parameters) noexcept;

  // Variables
  [[nodiscard]] static std::expected<std::shared_ptr<AST_AUTO_DECL>, Error>
  createAutoDecl(const std::string &id,
                 const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_CONST_DECL>, Error>
  createConstDecl(const std::string &id, const std::string &type,
                  const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_LET_DECL>, Error>
  createLetDecl(const std::string &id, const std::string &type,
                const std::shared_ptr<AST> &value) noexcept;

  [[nodiscard]] static std::expected<std::shared_ptr<AST_VAR_CALL>, Error>
  createVarCall(const std::string &id) noexcept;

  // Chained expression
  [[nodiscard]] static std::expected<std::shared_ptr<AST_CHAINED>, Error>
  createChained(const std::shared_ptr<AST> &base,
                const std::vector<std::shared_ptr<AST>> &operations) noexcept;

  // Tree
  [[nodiscard]] static std::expected<std::shared_ptr<Tree>, Error>
  createTree(const std::shared_ptr<AST_BODY> &statements) noexcept;
};

} // namespace nicole

#endif