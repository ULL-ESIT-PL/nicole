#include "../../../inc/visitors/fillSemanticInfo/fillSemanticInfo.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_attrAccess.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_constructorCall.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_methodCall.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_struct.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_this.h"
#include <cstddef>
#include <memory>
#include <variant>

namespace nicole {

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRUCT");
  }

  analyzingInsideClass = true;

  currentStructGenericList_ = node->generics();

  if (hasDuplicatedGenerics(currentStructGenericList_)) {
    return createError(ERROR_TYPE::TYPE, "has duplicated generics");
  }

  std::shared_ptr<UserType> father{nullptr};
  if (node->fatherType()) {
    const auto instanceType =
        std::dynamic_pointer_cast<GenericInstanceType>(node->fatherType());
    if (instanceType) {
      if (typeTable_->isGenericType(instanceType, currentStructGenericList_)) {
        return createError(ERROR_TYPE::TYPE,
                           instanceType->toString() +
                               " is a generic so you cannot extend from it");
      }
      auto fatherExpected = typeTable_->getType(instanceType->name());
      if (!fatherExpected) {
        return createError(fatherExpected.error());
      }
      father = std::dynamic_pointer_cast<UserType>(fatherExpected.value());
      if (!father) {
        return createError(ERROR_TYPE::TYPE,
                           "The father type is not a UserType");
      }
    } else {
      const auto userType =
          std::dynamic_pointer_cast<UserType>(node->fatherType());
      if (!userType) {
        return createError(
            ERROR_TYPE::TYPE,
            "father type can only be a user type or generic instance");
      }
      if (typeTable_->isGenericType(userType, currentStructGenericList_)) {
        return createError(ERROR_TYPE::TYPE,
                           userType->toString() +
                               " is a generic so you cannot extend from it");
      }
      auto fatherExpected = typeTable_->getType(userType->name());
      if (!fatherExpected) {
        return createError(fatherExpected.error());
      }
      father = std::dynamic_pointer_cast<UserType>(fatherExpected.value());
      if (!father) {
        return createError(ERROR_TYPE::TYPE,
                           "The father type is not a UserType");
      }
    }
  }

  currentUserType_ =
      std::make_shared<UserType>(node->id(), father, node->generics());

  const auto insertType{typeTable_->insert(currentUserType_)};
  if (!insertType) {
    return createError(insertType.error());
  }

  pushScope();

  size_t pos{0};
  for (const auto &attr : node->attributes()) {
    const auto insert{
        currentUserType_->insertAttr(Attribute{attr.first, attr.second, pos})};
    if (!insert) {
      return createError(insert.error());
    }
    ++pos;
  }

  for (const auto &method : node->methods()) {
    const auto result{method->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }

  const auto constructor{node->constructor()->accept(*this)};
  if (!constructor) {
    return createError(constructor.error());
  }

  const auto destructor{node->destructor()->accept(*this)};
  if (!destructor) {
    return createError(destructor.error());
  }

  popScope();

  analyzingInsideClass = false;
  currentStructGenericList_.clear();
  currentGenericList_.clear();

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_ATTR_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ATTR_ACCESS");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_METHOD_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_CALL");
  }

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_METHOD_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_DECL");
  }

  currentGenericList_ = node->generics();

  const auto genrics{
      mergeGenericLists(currentStructGenericList_, currentGenericList_)};
  if (!genrics) {
    return createError(genrics.error());
  }

  currentGenericList_ = *genrics;

  Method newMethod{node->id(),         currentGenericList_, node->parameters(),
                   node->returnType(), node->body(),        node->isVirtual()};

  const auto combinedMethods{currentUserType_->getMethods(node->id())};
  if (combinedMethods) {
    for (const auto &existingMethod : *combinedMethods) {
      if (areAmbiguousMethods(existingMethod, newMethod)) {
        if (!existingMethod.isInherited()) {
          return createError(ERROR_TYPE::METHOD,
                             "Ambiguous method declaration for: " + node->id());
        }
        if (existingMethod.isInherited() && !existingMethod.isVirtual()) {
          return createError(ERROR_TYPE::METHOD,
                             "Ambiguous method declaration for: " + node->id());
        }
      }
    }
  }

  currentUserType_->insertMethod(newMethod);

  pushScope();
  node->body()->setScope(currentScope_);

  for (const auto &param : node->parameters()) {
    if (currentUserType_->hasAttribute(param.first)) {
      return createError(ERROR_TYPE::ATTR, " the variable " + param.first +
                                               " is shadowing the atribute");
    }

    if (!typeTable_->isPossibleType(param.second) and
        !typeTable_->isGenericType(param.second, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         param.second->toString() +
                             " is not a posibble type or generic");
    }

    const auto insert{
        currentScope_->insert(Variable{param.first, param.second, nullptr})};
    if (!insert) {
      return createError(insert.error());
    }
  }

  if (!typeTable_->isPossibleType(node->returnType()) and
      !typeTable_->isGenericType(node->returnType(), currentGenericList_)) {
    return createError(ERROR_TYPE::TYPE,
                       node->returnType()->toString() +
                           " is not a posibble type or generic");
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
FillSemanticInfo::visit(const AST_CONSTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_DECL");
  }

  currentGenericList_ = node->generics();

  const auto genrics{
      mergeGenericLists(currentStructGenericList_, currentGenericList_)};
  if (!genrics) {
    return createError(genrics.error());
  }

  currentGenericList_ = *genrics;

  currentUserType_->setConstructor(std::make_shared<Constructor>(
      node->id(), node->generics(), node->parameters(), node->returnType(),
      node->body()));

  pushScope();
  node->body()->setScope(currentScope_);

  for (const auto &param : node->parameters()) {
    if (currentUserType_->hasAttribute(param.first)) {
      return createError(ERROR_TYPE::ATTR, " the variable " + param.first +
                                               " is shadowing the atribute");
    }

    if (!typeTable_->isPossibleType(param.second) and
        !typeTable_->isGenericType(param.second, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         param.second->toString() +
                             " is not a posibble type or generic");
    }

    const auto insert{
        currentScope_->insert(Variable{param.first, param.second, nullptr})};
    if (!insert) {
      return createError(insert.error());
    }
  }

  if (node->super()) {
    const auto result{node->super()->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }

  if (!typeTable_->isPossibleType(node->returnType()) and
      !typeTable_->isGenericType(node->returnType(), currentGenericList_)) {
    return createError(ERROR_TYPE::TYPE,
                       node->returnType()->toString() +
                           " is not a posibble type or generic");
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
FillSemanticInfo::visit(const AST_SUPER *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SUPER");
  }
  for (const auto &replacement : node->replacements()) {
    if (!typeTable_->isPossibleType(replacement) and
        !typeTable_->isGenericType(replacement, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         replacement->toString() +
                             " is not a posibble type or generic");
    }
  }
  for (const auto &arg : node->arguments()) {
    const auto result{arg->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_DESTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_DESTRUCTOR_DECL");
  }

  currentUserType_->setDestructor(
      std::make_shared<Destructor>(node->id(), node->body()));

  pushScope();
  node->body()->setScope(currentScope_);

  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }

  popScope();

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_THIS");
  }
  if (analyzingInsideClass) {
    node->setUserType(currentUserType_);
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_CONSTRUCTOR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_CALL");
  }
  if (!typeTable_->getType(node->id())) {
    return createError(ERROR_TYPE::TYPE,
                       "no type with id: " + node->id() + " exists");
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

}