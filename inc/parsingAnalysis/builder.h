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
  createAdd(const std::shared_ptr<AST> &left, const std::shared_ptr<AST> &right,
            const SourceLocation &sourceLocation) noexcept;
};

} // namespace nicole

#endif