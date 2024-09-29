#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "genericType.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

#include <memory>
#include <string>

namespace nicole {

class UserType : public GenericType {
public:
  UserType(const std::string &name,
           std::shared_ptr<GenericType> father = nullptr)
      : GenericType{name, father} {};

  ~UserType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::StructType::getTypeByName(*context, name_);
  };
};

} // namespace nicole

#endif