#ifndef CHAR_TYPE_H
#define CHAR_TYPE_H

#include "genericType.h"
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include <memory>
#include <string>

namespace nicole {

class CharType : public GenericType {
public:
  CharType(std::shared_ptr<GenericType> father = nullptr)
      : GenericType{"char", father} {};

  ~CharType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::Type::getInt8Ty(*context);
  };
};

} // namespace nicole

#endif