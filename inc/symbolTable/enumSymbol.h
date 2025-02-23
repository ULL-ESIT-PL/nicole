#ifndef ENUM_SYMBOL_H
#define ENUM_SYMBOL_H

#include "symbol.h"
#include <string>
#include <vector>

namespace nicole {

class EnumSymbol final : public Symbol {
private:
  std::vector<std::string> enumerators_;

public:
  explicit EnumSymbol(const std::string &id, const SymbolKind kind,
                      const std::vector<std::string> &enumerators) noexcept
      : Symbol{id, kind}, enumerators_{enumerators} {}

  [[nodiscard]] const std::vector<std::string> &enumerators() const noexcept {
    return enumerators_;
  }
};

} // namespace nicole

#endif