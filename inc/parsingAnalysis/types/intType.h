#ifndef INT_TYPE_H
#define INT_TYPE_H

#include "genericType.h"
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include <memory>
#include <string>

namespace nicole {

// A primitive of our language
class IntType : public GenericType {
public:
  IntType(std::shared_ptr<GenericType> father = nullptr)
      : GenericType{"int", father} {};

  ~IntType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::Type::getInt32Ty(*context);
  };
};

} // namespace nicole

#endif