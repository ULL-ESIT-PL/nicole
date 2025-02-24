#ifndef TYPE_SYMBOL_H
#define TYPE_SYMBOL_H

#include "symbol.h"
#include <memory>
#include <string>
#include "../parsingAnalysis/types/type.h"

namespace nicole {

class TypeSymbol final : public Symbol {
private:
  std::shared_ptr<Type> type_;

public:
  explicit TypeSymbol(const std::string &id, 
                      const std::shared_ptr<Type> &type) noexcept
      : Symbol{id, SymbolKind::Type}, type_{type} {}

  [[nodiscard]] const std::shared_ptr<Type> &type() const noexcept {
    return type_;
  }
};

} // namespace nicole

#endif