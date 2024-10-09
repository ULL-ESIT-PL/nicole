#ifndef DOUBLE_TYPE_H
#define DOUBLE_TYPE_H

#include "genericType.h"
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include <memory>
#include <string>

namespace nicole {

// A primitive of our language
class DoubleType : public GenericType {
public:
  DoubleType(std::shared_ptr<GenericType> father = nullptr)
      : GenericType{"double", father} {};

  ~DoubleType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::Type::getDoubleTy(*context);
  };
};

} // namespace nicole

#endif