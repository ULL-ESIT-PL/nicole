#ifndef BASE_TYPE_H
#define BASE_TYPE_H

#include "../../../errors.h"
#include <expected>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <string>

namespace nicole {

class Type {
public:
  virtual ~Type() = default;

  [[nodiscard]] virtual std::string toString() const noexcept = 0;

  [[nodiscard]] virtual std::expected<llvm::Type *, Error>
  llvmVersion(llvm::LLVMContext &context) const noexcept = 0;
};

} // namespace nicole

#endif