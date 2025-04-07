#include "../../../inc/parsingAnalysis/ast/functions/ast_funcCall.h"
#include "../../../inc/parsingAnalysis/ast/functions/ast_funcDecl.h"
#include "../../../inc/parsingAnalysis/ast/functions/ast_return.h"
#include "../../../inc/visitors/fillSemanticInfo/fillSemanticInfo.h"
#include <memory>
#include <variant>

namespace nicole {

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_FUNC_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_FUNC_CALL");
  }

  if (functionTable_->getFunctions(node->id()).empty()) {
    return createError(ERROR_TYPE::FUNCTION,
                       "no function with id: " + node->id() + " exists");
  }

  for (const auto &replacement : node->replaceOfGenerics()) {
    if (!typeTable_->isPossibleType(replacement) and
        !typeTable_->isGenericType(replacement, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         replacement->toString() +
                             " is not a posibble type or generic");
    }
  }

  for (const auto &expr : node->parameters()) {
    const auto resul{expr->accept(*this)};
    if (!resul) {
      return createError(resul.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_FUNC_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FUNC_DECL");
  }

  const Function newFunction{Function{node->id(), node->generics(),
                                      node->parameters(), node->returnType(),
                                      node->body()}};

  const auto functions{functionTable_->getFunctions(newFunction.id())};
  for (const auto &func : functions) {
    if (areAmbiguousFunctions(newFunction, func)) {
      return createError(ERROR_TYPE::FUNCTION,
                         "redeclaration of function: " + newFunction.id());
    }
  }
  functionTable_->insert(newFunction);

  pushScope();
  node->body()->setScope(currentScope_);

  currentGenericList_ = node->generics();
  if (hasDuplicatedGenerics(currentGenericList_)) {
    return createError(ERROR_TYPE::FUNCTION, "has duplicated generics");
  }

  // Crear un nuevo vector para los parámetros actualizados.
  std::vector<std::pair<std::string, std::shared_ptr<nicole::Type>>>
      newParameters;
  for (const auto &param : node->parameters()) {
    if (!typeTable_->isPossibleType(param.second) &&
        !typeTable_->isGenericType(param.second, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         param.second->toString() +
                             " is not a possible type or generic");
    }
    auto newType = param.second;
    const auto checkIfMaskedEnum = typeTable_->isCompundEnumType(newType);
    if (checkIfMaskedEnum) {
      newType = *checkIfMaskedEnum;
    }
    newParameters.push_back({param.first, newType});
    const auto insertVariable{
        currentScope_->insert(Variable{param.first, newType, nullptr})};
    if (!insertVariable) {
      return createError(insertVariable.error());
    }
  }
  // Actualizar los parámetros del AST con el nuevo vector.
  Parameters params{newParameters};
  node->setParameters(params);
  const auto setted{functionTable_->setFuncParameters(node->id(), params)};
  if (!setted) {
    return createError(setted.error());
  }

  // Procesar el tipo de retorno: verificar y actualizar si es compound enum.
  if (!typeTable_->isPossibleType(node->returnType()) &&
      !typeTable_->isGenericType(node->returnType(), currentGenericList_)) {
    return createError(ERROR_TYPE::TYPE,
                       node->returnType()->toString() +
                           " is not a possible type or generic");
  }
  const auto checkReturnEnum =
      typeTable_->isCompundEnumType(node->returnType());
  if (checkReturnEnum) {
    node->setReturnType(*checkReturnEnum);
    const auto settedReturnType{functionTable_->setFuncReturnType(node->id(), *checkReturnEnum)};
    if (!settedReturnType) {
      return createError(settedReturnType.error());
    }
  }

  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }

  popScope();
  currentGenericList_.clear();

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_RETURN");
  }
  if (!node->expression()) {
    return {};
  }
  return node->expression()->accept(*this);
}

} // namespace nicole