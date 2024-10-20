#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "../ast/declaration/paramsDeclaration.h"
#include "genericType.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

#include <memory>
#include <string>
#include <vector>

namespace nicole {

// Used in structs and classes
class UserType : public GenericType {
private:
  std::shared_ptr<ParamsDeclaration> attributes_{nullptr};

public:
  UserType(const std::string &name,
           std::shared_ptr<GenericType> father = nullptr)
      : GenericType{name, father} {};

  ~UserType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::StructType::getTypeByName(*context, name_);
  };

  std::shared_ptr<ParamsDeclaration> attributes() const { return attributes_; }

  void setAttributes(std::shared_ptr<ParamsDeclaration> attributes) {
    attributes_ = attributes;
  };
};

} // namespace nicole

#endif