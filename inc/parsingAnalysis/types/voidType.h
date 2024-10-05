#ifndef VOID_TYPE_H
#define VOID_TYPE_H

#include "genericType.h"
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include <memory>
#include <string>

namespace nicole {

class VoidType : public GenericType {
public:
  VoidType(std::shared_ptr<GenericType> father = nullptr)
      : GenericType{"void", father} {};

  ~VoidType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::Type::getVoidTy(*context);
  };
};

} // namespace nicole

#endif