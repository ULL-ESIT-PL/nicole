#include "../../../inc/tables/typeTable/types/userTypes/userType.h"
#include <memory>

namespace nicole {

bool UserType::hasAttribute(const std::string &id) const noexcept {
  if (baseType_) {
    if (const auto userType = std::dynamic_pointer_cast<UserType>(baseType_)) {
      return userType->hasAttribute(id);
    }
  }
  if (attrTable_.has(id)) {
    return true;
  }
  return false;
}

bool UserType::hasMethod(const Method &id) const noexcept {
  if (baseType_) {
    if (const auto userType = std::dynamic_pointer_cast<UserType>(baseType_))
      return userType->hasMethod(id);
  }
  if (methodTable_.getMethods(id.id()).size()) {
    return true;
  }
  return false;
}

const std::expected<Attribute, Error>
UserType::getAttribute(const std::string &id) const noexcept {
  if (attrTable_.has(id)) {
    return attrTable_.getAttribute(id);
  }
  if (baseType_) {
    if (const auto userType = std::dynamic_pointer_cast<UserType>(baseType_))
      return userType->getAttribute(id);
  }
  return createError(ERROR_TYPE::ATTR,
                     "the attribute: " + id + " does not exist in " + name_);
}

const std::expected<std::vector<Method>, Error>
UserType::getMethods(const std::string &id) const noexcept {
  std::vector<Method> combinedMethods;
  if (methodTable_.getMethods(id).size()) {
    auto childRes = methodTable_.getMethods(id);
    combinedMethods.insert(combinedMethods.end(), childRes.begin(),
                           childRes.end());
  }
  if (baseType_) {
    if (const auto userType = std::dynamic_pointer_cast<UserType>(baseType_)) {
      auto parentRes = userType->getMethods(id);
      for (auto &method : parentRes.value()) {
        method.setInherit(true);
      }
      if (parentRes)
        combinedMethods.insert(combinedMethods.end(), parentRes.value().begin(),
                               parentRes.value().end());
    }
  }
  if (!combinedMethods.empty())
    return combinedMethods;

  return createError(ERROR_TYPE::METHOD,
                     "the method: " + id + " does not exist in " + name_);
}

std::expected<std::monostate, Error>
UserType::insertAttr(const Attribute &attr) const noexcept {
  if (hasAttribute(attr.id())) {
    return createError(ERROR_TYPE::ATTR,
                       "the attribute: " + attr.id() + " already exists");
  }
  return attrTable_.insert(attr);
}

void UserType::insertMethod(const Method &method) const noexcept {
  methodTable_.insert(method);
}

bool UserType::isAboveInHearchy(
    const std::shared_ptr<UserType> &type) const noexcept {
  auto aux{baseType_};
  while (aux) {
    if (aux->name_ == type->name_) {
      return true;
    }
    aux = aux->baseType_;
  }
  return false;
}

std::string UserType::toString() const noexcept {
  std::ostringstream oss;
  oss << name_;
  if (!genericParams_.empty()) {
    oss << "<";
    for (size_t i = 0; i < genericParams_.size(); ++i) {
      oss << genericParams_[i].name();
      if (i != genericParams_.size() - 1)
        oss << ", ";
    }
    oss << ">";
  }
  if (baseType_) {
    oss << " : " << baseType_->toString();
  }
  return oss.str();
}

} // namespace nicole