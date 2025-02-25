#ifndef VARIABLE_H
#define VARIABLE_H

#include "../../parsingAnalysis/types/type.h"
#include <expected>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <string>

namespace nicole {

class Variable final {
private:
  std::string id_;
  mutable std::shared_ptr<Type> type_;
  mutable std::shared_ptr<llvm::Value> value_;
  mutable std::shared_ptr<llvm::AllocaInst> address_;

public:
  explicit Variable(const std::string &id, const std::shared_ptr<Type> &type,
                    const std::shared_ptr<llvm::Value> &value) noexcept
      : id_{id}, type_{type}, value_{value} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::shared_ptr<Type> &type() const noexcept {
    return type_;
  }

  [[nodiscard]] const std::shared_ptr<llvm::Value> &value() const noexcept {
    return value_;
  }

  [[nodiscard]] const std::shared_ptr<llvm::AllocaInst> &
  address() const noexcept {
    return address_;
  }
};

} // namespace nicole

#endif