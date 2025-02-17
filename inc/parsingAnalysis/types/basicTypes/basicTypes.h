#ifndef CONST_TYPE_H
#define CONST_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

enum class BasicKind { Bool, Int, Float, Double, Char, Str };

class BasicType : public Type {
  BasicKind kind;

public:
  explicit BasicType(BasicKind k) : kind(k) {}
  
  [[nodiscard]] std::string toString() const noexcept override {
    switch (kind) {
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