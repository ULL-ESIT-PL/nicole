#include "../../inc/parsingAnalysis/builder.h"

namespace nicole {

std::shared_ptr<AST_BOOL>
Builder::createBool(const bool value,
                    const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_BOOL>(value, father);
}

std::shared_ptr<AST_CHAR>
Builder::createChar(const char value,
                    const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_CHAR>(value, father);
}

std::shared_ptr<AST_DOUBLE>
Builder::createDouble(const double value,
                      const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_DOUBLE>(value, father);
}

std::shared_ptr<AST_FLOAT>
Builder::createFloat(const float value,
                     const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_FLOAT>(value, father);
}

std::shared_ptr<AST_INT>
Builder::createInt(const int value,
                   const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_INT>(value, father);
}

std::shared_ptr<AST_NULL>
Builder::createNull(const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_NULL>(father);
}

std::shared_ptr<AST_STRING>
Builder::createString(const std::string value,
                      const std::shared_ptr<AST> &father) noexcept {
  return std::make_shared<AST_STRING>(value, father);
}

} // namespace nicole