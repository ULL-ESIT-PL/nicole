#include "../../../inc/visitors/fillSemanticInfo/fillSemanticInfo.h"
#include "../../../inc/parsingAnalysis/ast/enum/ast_enum.h"
#include "../../../inc/parsingAnalysis/ast/enum/ast_enumAccess.h"
#include <memory>
#include <variant>

namespace nicole {

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  const auto insert{typeTable_->insert(
      std::make_shared<EnumType>(node->id(), node->identifiers()))};
  if (!insert) {
    return createError(insert.error());
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_ENUM_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM_ACCESS");
  }
  const auto typeExists{typeTable_->getType(node->enumId())};
  if (!typeExists) {
    return createError(typeExists.error());
  }
  const auto enumType{std::dynamic_pointer_cast<EnumType>(*typeExists)};
  if (!enumType) {
    return createError(
        ERROR_TYPE::TYPE,
        "attempting to access a type as a enum but it isn't enum");
  }
  if (!enumType->hasIdentifier(node->identifier())) {
    return createError(ERROR_TYPE::TYPE, "the enum " + enumType->name() +
                                             " does not have the identifier " +
                                             node->identifier());
  }
  return {};
}

}