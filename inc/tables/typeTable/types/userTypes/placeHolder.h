#ifndef PLACE_HOLDER_H
#define PLACE_HOLDER_H

#include "../type.h"
#include "genericParameter.h" // Incluir el header de GenericParameter
#include <string>

namespace nicole {

class PlaceHolder final : public Type {
private:
  GenericParameter genericParam_; // Almacena el parámetro genérico asociado

public:
  explicit PlaceHolder(const GenericParameter &genericParam) noexcept
      : genericParam_(genericParam) {}

  [[nodiscard]] std::string toString() const noexcept override {
    return "PlaceHolder: " + genericParam_.name();
  }

  // Se puede añadir un getter si se requiere acceder al GenericParameter
  [[nodiscard]] const GenericParameter &getGenericParameter() const noexcept {
    return genericParam_;
  }
};

} // namespace nicole

#endif
