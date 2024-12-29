#include "../../inc/parsingAnalysis/builder.h"

namespace nicole {

std::shared_ptr<AST_BOOL>
Builder::createBool(const bool value, const SourceLocation &sourceLocation,
                    const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_BOOL>(value, sourceLocation, father);
}

std::shared_ptr<AST_CHAR>
Builder::createChar(const char value, const SourceLocation &sourceLocation,
                    const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_CHAR>(value, sourceLocation, father);
}

std::shared_ptr<AST_DOUBLE>
Builder::createDouble(const double value, const SourceLocation &sourceLocation,
                      const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_DOUBLE>(value, sourceLocation, father);
}

std::shared_ptr<AST_FLOAT>
Builder::createFloat(const float value, const SourceLocation &sourceLocation,
                     const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_FLOAT>(value, sourceLocation, father);
}

std::shared_ptr<AST_INT>
Builder::createInt(const int value, const SourceLocation &sourceLocation,
                   const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_INT>(value, sourceLocation, father);
}

std::shared_ptr<AST_NULL>
Builder::createNull(const SourceLocation &sourceLocation,
                    const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_NULL>(sourceLocation, father);
}

std::shared_ptr<AST_STRING>
Builder::createString(const std::string value,
                      const SourceLocation &sourceLocation,
                      const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_STRING>(value, sourceLocation, father);
}

} // namespace nicole