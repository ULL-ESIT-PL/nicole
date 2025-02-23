#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "symbol.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace nicole {

class SymbolTable final {
private:
  std::unordered_map<std::string, std::vector<std::shared_ptr<Symbol>>>
      symbols_;

public:
  void addSymbol(const std::shared_ptr<Symbol> &symbol) noexcept {
    symbols_[symbol->id()].push_back(symbol);
  }

  [[nodiscard]] const std::unordered_map<std::string,
                                         std::vector<std::shared_ptr<Symbol>>> &
  dataStructure() const noexcept {
    return symbols_;
  }

  [[nodiscard]] std::unordered_map<std::string,
                                   std::vector<std::shared_ptr<Symbol>>> &
  dataStructure() noexcept {
    return symbols_;
  }

  [[nodiscard]] std::vector<std::shared_ptr<Symbol>>
  getSymbols(const std::string &name) const noexcept {
    auto it = symbols_.find(name);
    return (it != symbols_.end()) ? it->second
                                  : std::vector<std::shared_ptr<Symbol>>{};
  }
};

} // namespace nicole

#endif