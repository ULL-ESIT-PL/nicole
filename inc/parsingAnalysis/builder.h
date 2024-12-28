#ifndef BUILDER_H
#define BUILDER_H

#include "ast/literals/ast_bool.h"
#include "ast/literals/ast_char.h"
#include "ast/literals/ast_double.h"
#include "ast/literals/ast_float.h"
#include "ast/literals/ast_int.h"
#include "ast/literals/ast_null.h"
#include "ast/literals/ast_string.h"
#include <expected>

namespace nicole {

class Builder final {
private:
  Builder() = delete;

public:
  [[nodiscard]] static std::shared_ptr<AST_BOOL>
  createBool(const bool value, const std::shared_ptr<AST> &father);

  [[nodiscard]] static std::shared_ptr<AST_CHAR>
  createChar(const char value, const std::shared_ptr<AST> &father);

  [[nodiscard]] static std::shared_ptr<AST_DOUBLE>
  createDouble(const double value, const std::shared_ptr<AST> &father);

  [[nodiscard]] static std::shared_ptr<AST_FLOAT>
  createFloat(const float value, const std::shared_ptr<AST> &father);

  [[nodiscard]] static std::shared_ptr<AST_INT>
  createInt(const int value, const std::shared_ptr<AST> &father);

  [[nodiscard]] static std::shared_ptr<AST_NULL>
  createNull(const std::shared_ptr<AST> &father);

  [[nodiscard]] static std::shared_ptr<AST_STRING>
  createString(const std::string value, const std::shared_ptr<AST> &father);
};

} // namespace nicole

#endif