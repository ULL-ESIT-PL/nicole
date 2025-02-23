#ifndef FUNCTION_SYMBOL_H
#define FUNCTION_SYMBOL_H

#include "../parsingAnalysis/types/type.h"
#include "../parsingAnalysis/types/userTypes/genericParameter.h"
#include "variableSymbol.h"
#include <memory>
#include <string>
#include <vector>

namespace nicole {

class FunctionSymbol final : public Symbol {
private:
  std::vector<GenericParameter> generics_;
  std::vector<std::shared_ptr<VariableSymbol>> params_;
  std::shared_ptr<Type> returnType_;

public:
  explicit FunctionSymbol(
      const std::string &id, const SymbolKind kind,
      const std::vector<GenericParameter> &generics,
      const std::vector<std::shared_ptr<VariableSymbol>> &params,
      const std::shared_ptr<Type> &returnType) noexcept
      : Symbol{id, kind}, generics_{generics}, params_{params},
        returnType_{returnType} {}

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<VariableSymbol>> &
  params() const noexcept {
    return params_;
  }

  [[nodiscard]] const std::shared_ptr<Type> &returnType() const noexcept {
    return returnType_;
  }
};

} // namespace nicole

#endif