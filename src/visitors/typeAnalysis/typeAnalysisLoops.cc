#include "../../../inc/visitors/typeAnalysis/typeAnalysis.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_doWhile.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_for.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_pass.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_stop.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_while.h"
#include <memory>

namespace nicole {

/*
- condtion debe retornar bool
- si body encuentra algo diferente a NoPropagate lo propaga si no retorna
NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_WHILE");
  }
  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();
  
  /*
  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);
  */
  auto boolType = typeTable_->getType("bool");
  if (!typeTable_->areSameType(condType, *boolType))
    return createError(ERROR_TYPE::TYPE, "a condition must be boolean");

  auto body = node->body()->accept(*this);
  if (!body)
    return createError(body.error());
  auto bodyType = body.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    return bodyType;
  }

  return typeTable_->noPropagateType();
}

/*
- init ???
- condtion reotrna bool
- update ???
- body lo mismo que los otros
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FOR");
  }
  if (!node->init().empty()) {
    for (const auto &expr : node->init()) {
      auto initResult = expr->accept(*this);
      if (!initResult)
        return createError(initResult.error());
    }
  }

  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();

  /*
  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);
  */
  auto boolType = typeTable_->getType("bool");
  if (!typeTable_->areSameType(condType, *boolType))
    return createError(ERROR_TYPE::TYPE, "a condition must be boolean");

  if (!node->update().empty()) {
    for (const auto &expr : node->update()) {
      auto updateResult = expr->accept(*this);
      if (!updateResult)
        return createError(updateResult.error());
    }
  }

  auto body = node->body()->accept(*this);
  if (!body)
    return createError(body.error());
  auto bodyType = body.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    return bodyType;
  }

  return typeTable_->noPropagateType();
}

/*
- si body encuentra algo diferente a NoPropagate lo propaga si no retorna
NoPropagate
- condtion debe retornar bool
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DO_WHILE");
  }
  auto body = node->body()->accept(*this);
  if (!body)
    return createError(body.error());
  auto bodyType = body.value();

  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();
  /*
  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);
  */
  auto boolType = typeTable_->getType("bool");
  if (!typeTable_->areSameType(condType, *boolType))
    return createError(ERROR_TYPE::TYPE, "a condition must be boolean");

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    return bodyType;
  }

  return typeTable_->noPropagateType();
}

/*
- retorna BreakType
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PASS");
  }
  return typeTable_->breakType();
}

/*
- retorna BreakType
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STOP");
  }
  return typeTable_->breakType();
}

}