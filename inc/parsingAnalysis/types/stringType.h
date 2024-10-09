#ifndef STRING_TYPE_H
#define STRING_TYPE_H

#include "genericType.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

#include <memory>
#include <string>

namespace nicole {

// A primitive of our language
class StringType : public GenericType {
public:
  StringType(std::shared_ptr<GenericType> father = nullptr)
      : GenericType{"str", father} {};

  ~StringType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*context), 0);
  };
};

} // namespace nicole

#endif