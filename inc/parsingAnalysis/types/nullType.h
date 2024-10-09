#ifndef NULL_TYPE_H
#define NULL_TYPE_H

#include "genericType.h"
#include "llvm/Support/ErrorHandling.h"
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include <memory>
#include <string>

namespace nicole {

// A primitive of our language
class NullType : public GenericType {
public:
  NullType(std::shared_ptr<GenericType> father = nullptr)
      : GenericType{"null", father} {};

  ~NullType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    llvm::report_fatal_error("null not implemented");
  };
};

} // namespace nicole

#endif