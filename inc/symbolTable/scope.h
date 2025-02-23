#ifndef SCOPE_H
#define SCOPE_H

#include "symbolTable.h"
#include <memory>
#include <string>
#include <vector>

namespace nicole {

class Scope {
private:
  std::unique_ptr<SymbolTable> symbolTable_;
  std::shared_ptr<Scope> parent_;

public:
  explicit Scope(std::shared_ptr<Scope> parent = nullptr) noexcept
      : parent_(parent), symbolTable_(std::make_unique<SymbolTable>()) {}

  void addSymbol(const std::shared_ptr<Symbol> &symbol) {
    symbolTable_->addSymbol(symbol);
  }

  std::vector<std::shared_ptr<Symbol>> lookup(const std::string &name) const {
    auto result = symbolTable_->getSymbols(name);
    if (parent_) {
      auto parentResult = parent_->lookup(name);
      result.insert(result.end(), parentResult.begin(), parentResult.end());
    }
    return result;
  }
};

} // namespace nicole

#endif // SCOPE_H
