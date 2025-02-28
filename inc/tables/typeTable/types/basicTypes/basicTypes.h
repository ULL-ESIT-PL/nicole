#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

enum class BasicKind { Bool, Int, Float, Double, Char, Str };

class BasicType : public Type {
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
};

} // namespace nicole

#endif