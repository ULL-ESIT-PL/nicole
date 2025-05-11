#ifndef PLACE_HOLDER_H
#define PLACE_HOLDER_H

#include "../type.h"
#include "genericParameter.h"
#include <string>

namespace nicole {

class PlaceHolder final : public Type {
private:
  GenericParameter genericParameter_;

public:
  explicit PlaceHolder(const GenericParameter &genericParameter) noexcept
      : genericParameter_(genericParameter) {}

  [[nodiscard]] std::string toString() const noexcept override {
    return "PlaceHolder: " + genericParameter_.name();
  }

  [[nodiscard]] const GenericParameter &getGenericParameter() const noexcept {
    return genericParameter_;
  }

  [[nodiscard]] std::expected<llvm::Type *, Error>
  llvmVersion(llvm::LLVMContext &) const noexcept override {
    // Unresolved placeholder: no LLVM type can be generated
    return createError(ERROR_TYPE::TYPE,
                       "Unresolved generic parameter '" +
                           genericParameter_.name() +
                           "' cannot be lowered to LLVM type.");
  }
};

} // namespace nicole

#endif
