#ifndef VARIABLE_H
#define VARIABLE_H

#include "../symbol.h"
#include "../typeTable/types/type.h"
#include <expected>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <string>

namespace nicole {

class Variable final : public Symbol {
private:
  mutable std::shared_ptr<Type> type_;
  mutable llvm::Value *value_;
  mutable llvm::AllocaInst *address_;

public:
  explicit Variable(const std::string &id, const std::shared_ptr<Type> &type,
                    llvm::Value *value) noexcept
      : Symbol{id}, type_{type}, value_{value} {}

  [[nodiscard]] const std::shared_ptr<Type> &type() const noexcept {
    return type_;
  }

  [[nodiscard]] llvm::Value *&value() const noexcept { return value_; }

  [[nodiscard]] llvm::AllocaInst *address() const noexcept { return address_; }

  void setType(const std::shared_ptr<Type> &type) const noexcept {
    type_ = type;
  }

  void setValue(llvm::Value *v) const noexcept { value_ = v; }

  void setAddress(llvm::AllocaInst *addr) const noexcept { address_ = addr; }
};

} // namespace nicole

#endif