#ifndef BOOL_TYPE_H
#define BOOL_TYPE_H

#include "genericType.h"
#include "llvm/IR/Constants.h"
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include <memory>
#include <string>

namespace nicole {

// A primitive of our language
class BoolType : public GenericType {
public:
  BoolType(std::shared_ptr<GenericType> father = nullptr)
      : GenericType{"bool", father} {};

  ~BoolType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::Type::getInt1Ty(*context);
  };
};

} // namespace nicole

#endif