#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#include "../type.h"
#include <string>

namespace nicole {

enum class BasicKind { Bool, Int, Float, Double, Char, Str };

class BasicType : public Type {
private:
  BasicKind kind_;

public:
  explicit BasicType(const BasicKind k) noexcept : kind_(k) {}

  [[nodiscard]] BasicKind baseKind() const noexcept { return kind_; }

  [[nodiscard]] std::string toString() const noexcept override {
    switch (kind_) {
    case BasicKind::Bool:
      return "bool";
    case BasicKind::Int:
      return "int";
    case BasicKind::Float:
      return "float";
    case BasicKind::Double:
      return "double";
    case BasicKind::Char:
      return "char";
    case BasicKind::Str:
      return "str";
    }
  }

  [[nodiscard]] std::expected<llvm::Type *, Error>
  llvmVersion(llvm::LLVMContext &context) const noexcept override {
    switch (kind_) {
    case BasicKind::Bool:
      return llvm::Type::getInt1Ty(context);
    case BasicKind::Int:
      return llvm::Type::getInt32Ty(context);
    case BasicKind::Float:
      return llvm::Type::getFloatTy(context);
    case BasicKind::Double:
      return llvm::Type::getDoubleTy(context);
    case BasicKind::Char:
      return llvm::Type::getInt8Ty(context);
    case BasicKind::Str: {
      // C-style string: pointer to i8
      llvm::Type *i8Ty = llvm::Type::getInt8Ty(context);
      return llvm::PointerType::get(i8Ty, /*AddressSpace=*/0);
    }
    }
  }
};

} // namespace nicole

#endif