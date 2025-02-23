#ifndef VARIABLE_SYMBOL_H
#define VARIABLE_SYMBOL_H

#include "symbol.h"
#include <memory>
#include <string>
#include "../parsingAnalysis/types/type.h"

namespace nicole {

class VariableSymbol final : public Symbol {
private:
  std::shared_ptr<Type> type_;

public:
  explicit VariableSymbol(const std::string &id, const SymbolKind kind,
                      const std::shared_ptr<Type> &type) noexcept
      : Symbol{id, kind}, type_{type} {}

  [[nodiscard]] const std::shared_ptr<Type> &type() const noexcept {
    return type_;
  }
};

} // namespace nicole

#endif