#ifndef NO_PROPAGATE_TYPE_H
#define NO_PROPAGATE_TYPE_H

#include "../type.h"
#include <string>

namespace nicole {

class NoPropagateType final : public Type {
public:
  [[nodiscard]] std::string toString() const noexcept override {
    return "NoPropagateType";
  }

  [[nodiscard]] std::expected<llvm::Type *, Error>
  llvmVersion(llvm::LLVMContext &) const noexcept override {
    return createError(ERROR_TYPE::TYPE, "invalid llvm conversion");
  }
};

} // namespace nicole

#endif