#ifndef VOID_TYPE_H
#define VOID_TYPE_H

#include "../type.h"
#include <string>

namespace nicole {

class VoidType final : public Type {
public:
  [[nodiscard]] std::string toString() const noexcept override {
    return "void";
  }

  [[nodiscard]] std::expected<llvm::Type*, Error>
  llvmVersion(llvm::LLVMContext &context) const noexcept override {
    // Representamos void en LLVM como el tipo VoidTy
    return llvm::Type::getVoidTy(context);
  }
};

} // namespace nicole

#endif