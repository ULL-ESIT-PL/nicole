#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/enum/ast_enum.h"
#include "../../../inc/parsingAnalysis/ast/enum/ast_enumAccess.h"
#include <memory>

namespace nicole {

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  return nullptr;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_ENUM_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM_ACCESS");
  }
  const auto type{typeTable_->getType(node->id())};
  if (!type) {
    return createError(ERROR_TYPE::TYPE, "enum does not exist");
  }
  const auto enumType{std::dynamic_pointer_cast<EnumType>(*type)};
  if (!enumType) {
    return createError(
        ERROR_TYPE::TYPE,
        "attempting to access a type as a enum but it isn't enum");
  }
  const auto conversion{enumType->identifierToNumber(node->identifier())};

  // Crear un ConstantInt de 32 bits con ese valor
  llvm::Type *intTy = llvm::Type::getInt32Ty(context_);
  llvm::Value *constInt =
      llvm::ConstantInt::get(intTy, *conversion, /*isSigned=*/true);

  // resultChainedExpression_ = constInt;
  return constInt;
}

} // namespace nicole