#ifndef FLOAT_TYPE_H
#define FLOAT_TYPE_H

#include "genericType.h"
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include <memory>
#include <string>

namespace nicole {

// A primitive of our language
class FloatType : public GenericType {
public:
  FloatType(std::shared_ptr<GenericType> father = nullptr)
      : GenericType{"float", father} {};

  ~FloatType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::Type::getFloatTy(*context);
  };
};

} // namespace nicole

#endif