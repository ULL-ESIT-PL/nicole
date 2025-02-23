#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

namespace nicole {

enum class SymbolKind { Type, Variable, Function, Enum };

class Symbol {
private:
  std::string id_;
  SymbolKind kind_;

public:
  explicit Symbol(const std::string &id, const SymbolKind kind) noexcept
      : id_{id}, kind_{kind} {}

  virtual ~Symbol() = default;

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] SymbolKind kind() const noexcept { return kind_; }
};

} // namespace nicole

#endif