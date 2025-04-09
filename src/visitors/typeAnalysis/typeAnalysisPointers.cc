#include "../../../inc/visitors/typeAnalysis/typeAnalysis.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_delete.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_deref.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_new.h"
#include <memory>

namespace nicole {

/*
- value debe ser un puntero y que este en la tabla de memoria dinamica
- retorna NoPropagate
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DELETE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DELETE");
  }

  auto result = node->value()->accept(*this);
  if (!result) {
    return createError(result.error());
  }

  auto type = result.value();

  if (insideDeclWithGenerics) {
    /*
    if (auto placeholder = std::dynamic_pointer_cast<PlaceHolder>(type)) {
      auto genericCompound = placeholder->getGenericCompound();
      if (std::dynamic_pointer_cast<PointerType>(genericCompound)) {
        return placeholder;
      }
      return createError(ERROR_TYPE::TYPE, "can only delete pointers");
    }
    */
  }

  if (auto constType = std::dynamic_pointer_cast<ConstType>(type)) {
    if (std::dynamic_pointer_cast<PointerType>(constType->baseType())) {
      return type;
    }
    return createError(ERROR_TYPE::TYPE, "can only delete pointers");
  }

  if (std::dynamic_pointer_cast<PointerType>(type)) {
    return typeTable_->noPropagateType();
  }

  return createError(ERROR_TYPE::TYPE, "can only delete pointers");
}

/*
- envuelve en un puntero la expression y lo retorna, debe ser constructor o
primitiva
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEW");
  }

  auto result = node->value()->accept(*this);
  if (!result)
    return createError(result.error());

  auto exprType = result.value();

  if (insideDeclWithGenerics and
      typeTable_->isGenericType(exprType, currentGenericList_))
    return std::make_shared<PointerType>(exprType);

  auto basicType = std::dynamic_pointer_cast<BasicType>(exprType);
  auto userType = std::dynamic_pointer_cast<UserType>(exprType);
  if (!basicType && !userType)
    return createError(
        ERROR_TYPE::TYPE,
        "can only use new with primitives, user types or generics");

  return std::make_shared<PointerType>(exprType);
}

/*
- la expresion debe ser un puntero y retorna el tipo al que apunta
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");
  }

  auto result = node->value()->accept(*this);
  if (!result)
    return createError(result.error());

  auto type = result.value();

  std::shared_ptr<Type> unwrappedType = type;
  if (auto constType = std::dynamic_pointer_cast<ConstType>(type))
    unwrappedType = constType->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(unwrappedType, currentGenericList_)) {
    /*
    if (auto ptrType = std::dynamic_pointer_cast<PointerType>(unwrappedType))
      return std::make_shared<PlaceHolder>(ptrType->baseType());
    return createError(ERROR_TYPE::TYPE, "can only deref a pointer");
    */
  }

  if (auto ptrType = std::dynamic_pointer_cast<PointerType>(unwrappedType))
    return ptrType->baseType();

  return createError(ERROR_TYPE::TYPE, "can only deref a pointer");
}

}