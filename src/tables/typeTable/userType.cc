#include "../../../inc/tables/typeTable/types/userTypes/userType.h"

namespace nicole {

bool UserType::hasAttribute(const std::string &id) const noexcept {
  if (attrTable_.has(id)) {
    return true;
  }
  if (baseType_) {
    if (const auto userType = std::dynamic_pointer_cast<UserType>(baseType_)) {
      std::cout << " hola";
      return userType->hasAttribute(id);
    }
  }
  return false;
}

bool UserType::hasMethod(const Method &id) const noexcept {
  if (methodTable_.has(id)) {
    return true;
  }
  if (baseType_) {
    if (const auto userType = std::dynamic_pointer_cast<UserType>(baseType_))
      return userType->hasMethod(id);
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

const std::expected<Method, Error>
UserType::getMethod(const std::string &id,
                    const Parameters &params) const noexcept {
  if (methodTable_.has(Method{id, {}, params, nullptr})) {
    return methodTable_.getMethod(id, params);
  }
  if (baseType_) {
    if (const auto userType = std::dynamic_pointer_cast<UserType>(baseType_))
      return userType->getMethod(id, params);
  }
  return createError(ERROR_TYPE::METHOD,
                     "the method: " + id + " does not exist in " + name_ +
                         " with the given parameters does not exist");
}

const std::expected<std::vector<Method>, Error>
UserType::getMethods(const std::string &id) const noexcept {
  std::vector<Method> combinedMethods;
  if (methodTable_.hasSymbol(id)) {
    auto childRes = methodTable_.getMethods(id);
    if (childRes)
      combinedMethods.insert(combinedMethods.end(), childRes.value().begin(),
                             childRes.value().end());
  }
  if (baseType_) {
    if (const auto userType = std::dynamic_pointer_cast<UserType>(baseType_)) {
      auto parentRes = userType->getMethods(id);
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

std::expected<std::monostate, Error>
UserType::insertMethod(const Method &method) const noexcept {
  if (hasMethod(method)) {
    return createError(ERROR_TYPE::METHOD,
                       "the method: " + method.id() + " already exists");
  }
  return methodTable_.insert(method);
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