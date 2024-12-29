#include "../../inc/parsingAnalysis/builder.h"

namespace nicole {

std::shared_ptr<AST_BOOL>
Builder::createBool(const bool value, const SourceLocation &sourceLocation
                    ) noexcept {
  return std::make_shared<AST_BOOL>(value, sourceLocation);
}

std::shared_ptr<AST_CHAR>
Builder::createChar(const char value, const SourceLocation &sourceLocation
                    ) noexcept {
  return std::make_shared<AST_CHAR>(value, sourceLocation);
}

std::shared_ptr<AST_DOUBLE>
Builder::createDouble(const double value, const SourceLocation &sourceLocation
                      ) noexcept {
  return std::make_shared<AST_DOUBLE>(value, sourceLocation);
}

std::shared_ptr<AST_FLOAT>
Builder::createFloat(const float value, const SourceLocation &sourceLocation
                     ) noexcept {
  return std::make_shared<AST_FLOAT>(value, sourceLocation);
}

std::shared_ptr<AST_INT>
Builder::createInt(const int value, const SourceLocation &sourceLocation
                   ) noexcept {
  return std::make_shared<AST_INT>(value, sourceLocation);
}

std::shared_ptr<AST_NULL>
Builder::createNull(const SourceLocation &sourceLocation
                    ) noexcept {
  return std::make_shared<AST_NULL>(sourceLocation);
}

std::shared_ptr<AST_STRING>
Builder::createString(const std::string value,
                      const SourceLocation &sourceLocation
                      ) noexcept {
  return std::make_shared<AST_STRING>(value, sourceLocation);
}

std::shared_ptr<AST_ADD>
Builder::createAdd(const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation
                   ) noexcept {
  const auto astAdd{std::make_shared<AST_ADD>(left, right, sourceLocation)};
  left->setFather(astAdd);
  right->setFather(astAdd);
  return astAdd;
}

} // namespace nicole