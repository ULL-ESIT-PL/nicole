#ifndef ENUM_TYPE_H
#define ENUM_TYPE_H

#include "../type.h"
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

namespace nicole {

class EnumType final : public Type {
private:
  std::string name_;
  std::vector<std::string> values_;

public:
  EnumType(const std::string &name,
           const std::vector<std::string> &values) noexcept
      : name_(name), values_(values) {}

  [[nodiscard]] const std::string &name() const noexcept { return name_; }

  [[nodiscard]] const std::vector<std::string> &values() const noexcept {
    return values_;
  }

  [[nodiscard]] bool
  hasIdentifier(const std::string &identifier) const noexcept {
    return std::find(values_.begin(), values_.end(), identifier) !=
           values_.end();
  }

  [[nodiscard]] std::expected<size_t, Error>
  identifierToNumber(const std::string &identifier) const noexcept {
    for (size_t i{0}; i < values_.size(); ++i) {
      if (identifier == values_[i]) {
        return i;
      }
    }
    return createError(ERROR_TYPE::TYPE,
                       "unable to convert to number identifier " + identifier +
                           "in enum: " + name_);
  }

  [[nodiscard]] std::string toString() const noexcept override {
    std::ostringstream oss;
    oss << "enum " << name_ << " { ";
    for (size_t i = 0; i < values_.size(); ++i) {
      oss << values_[i];
      if (i != values_.size() - 1) {
        oss << ", ";
      }
    }
    oss << " }";
    return oss.str();
  }

  [[nodiscard]] std::expected<llvm::Type *, Error>
  llvmVersion(llvm::LLVMContext &context) const noexcept override {
    // Representamos enum como i32
    return llvm::Type::getInt32Ty(context);
  }
};

} // namespace nicole

#endif
