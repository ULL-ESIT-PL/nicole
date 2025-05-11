#ifndef POINTER_TYPE_H
#define POINTER_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

class PointerType final : public Type {
private:
  std::shared_ptr<Type> baseType_;

public:
  explicit PointerType(const std::shared_ptr<Type> &baseType)
      : baseType_{baseType} {}

  [[nodiscard]] const std::shared_ptr<Type> &baseType() const noexcept {
    return baseType_;
  }

  [[nodiscard]] std::string toString() const noexcept override {
    return baseType_->toString() + "*";
  }

  [[nodiscard]] std::expected<llvm::Type *, Error>
  llvmVersion(llvm::LLVMContext &context) const noexcept override {
    // Obtener el tipo LLVM del tipo base
    std::expected<llvm::Type *, Error> baseTyOrErr =
        baseType_->llvmVersion(context);
    if (!baseTyOrErr) {
      return std::unexpected(baseTyOrErr.error());
    }
    llvm::Type *baseTy = *baseTyOrErr;
    // Crear y retornar el puntero al tipo base
    return llvm::PointerType::get(baseTy, /*AddressSpace=*/0);
  }
};

} // namespace nicole

#endif