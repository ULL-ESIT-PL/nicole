#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "attrTable.h"
#include "attribute.h"
#include "constructor.h"
#include "destructor.h"
#include "genericParameter.h"
#include "method.h"
#include "methodTable.h"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace nicole {

class UserType final : public Type {
private:
  std::string name_;
  std::shared_ptr<Type> baseType_; // Solo se permite una base
  std::vector<GenericParameter> genericParams_;
  mutable AttrTable attrTable_;
  mutable MethodTable methodTable_;
  mutable std::shared_ptr<Constructor> constructor_;
  mutable std::shared_ptr<Destructor> destructor_;

public:
  explicit UserType(const std::string &name,
                    const std::shared_ptr<Type> &baseType,
                    const std::vector<GenericParameter> &genericParams) noexcept
      : name_{name}, baseType_(baseType), genericParams_{genericParams} {}

  [[nodiscard]] const std::string &name() const noexcept { return name_; }

  [[nodiscard]] const std::shared_ptr<Type> &baseType() const noexcept {
    return baseType_;
  }

  [[nodiscard]] const std::vector<GenericParameter> &
  genericParams() const noexcept {
    return genericParams_;
  }

  [[nodiscard]] bool hasAttribute(const std::string &id) const noexcept {
    if (attrTable_.has(id)) {
      return true;
    }
    if (baseType_) {
      if (const auto userType = static_cast<UserType *>(baseType_.get()))
        return userType->hasAttribute(id);
    }
    return false;
  }

  [[nodiscard]] bool hasMethod(const Method &id) const noexcept {
    if (methodTable_.has(id)) {
      return true;
    }
    if (baseType_) {
      if (const auto userType = static_cast<UserType *>(baseType_.get()))
        return userType->hasMethod(id);
    }
    return false;
  }

  [[nodiscard]] const std::expected<Attribute, Error>
  getAttribute(const std::string &id) const noexcept {
    if (attrTable_.has(id)) {
      return attrTable_.getAttribute(id);
    }
    if (baseType_) {
      if (const auto userType = static_cast<UserType *>(baseType_.get()))
        return userType->getAttribute(id);
    }
    return createError(ERROR_TYPE::ATTR,
                       "the attribute: " + id + " does not exist in " + name_);
  }

  [[nodiscard]] const std::expected<Method, Error>
  getMethod(const std::string &id, const Parameters &params) const noexcept {
    if (methodTable_.has(Method{id, {}, params, nullptr})) {
      return methodTable_.getMethod(id, params);
    }
    if (baseType_) {
      if (const auto userType = static_cast<UserType *>(baseType_.get()))
        return userType->getMethod(id, params);
    }
    return createError(ERROR_TYPE::METHOD,
                       "the method: " + id + " does not exist in " + name_ +
                           " with the given parameters does not exist");
  }

  [[nodiscard]] const std::expected<std::vector<Method>, Error>
  getMethods(const std::string &id) const noexcept {
    std::vector<Method> combinedMethods;
    if (methodTable_.hasSymbol(id)) {
      auto childRes = methodTable_.getMethods(id);
      if (childRes)
        combinedMethods.insert(combinedMethods.end(), childRes.value().begin(),
                               childRes.value().end());
    }
    if (baseType_) {
      if (const auto userType = static_cast<UserType *>(baseType_.get())) {
        auto parentRes = userType->getMethods(id);
        if (parentRes)
          combinedMethods.insert(combinedMethods.end(),
                                 parentRes.value().begin(),
                                 parentRes.value().end());
      }
    }
    if (!combinedMethods.empty())
      return combinedMethods;

    return createError(ERROR_TYPE::METHOD,
                       "the method: " + id + " does not exist in " + name_);
  }

  void setAttrTable(const AttrTable &tb) const noexcept { attrTable_ = tb; }

  void
  setDestructor(const std::shared_ptr<Destructor> &destructor) const noexcept {
    destructor_ = destructor;
  }

  void
  setConstructor(const std::shared_ptr<Constructor> &constructor) const noexcept {
    constructor_ = constructor;
  }

  void setMethodTable(const MethodTable &tb) const noexcept {
    methodTable_ = tb;
  }

  [[nodiscard]] std::string toString() const noexcept override {
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
};

} // namespace nicole

#endif