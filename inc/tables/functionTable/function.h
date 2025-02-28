#ifndef FUNCTION_H
#define FUNCTION_H

#include "../../parsingAnalysis/ast/functions/parameters.h"
#include "../../parsingAnalysis/types/userTypes/genericParameter.h"
#include <llvm/IR/Function.h>
#include <memory>
#include "../symbol.h"

namespace nicole {

class Function final : public Symbol {
private:
  std::vector<GenericParameter> generics_;
  Parameters params_;
  std::shared_ptr<Type> returnType_;
  mutable std::shared_ptr<llvm::Function> llvmVersion_;

public:
  explicit Function(const std::string &id,
                    const std::vector<GenericParameter> &generics,
                    const Parameters &params,
                    const std::shared_ptr<Type> &returnType) noexcept
      : Symbol{id}, generics_{generics}, params_{params}, returnType_{returnType} {
  }

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  [[nodiscard]] const Parameters &params() const noexcept { return params_; }

  [[nodiscard]] const std::shared_ptr<Type> &returnType() const noexcept {
    return returnType_;
  }

  [[nodiscard]] const std::shared_ptr<llvm::Function> &
  llvmVersion() const noexcept {
    return llvmVersion_;
  }

  void
  setLlvmVersion(const std::shared_ptr<llvm::Function> &llvmVersion) noexcept {
    llvmVersion_ = llvmVersion;
  }

  [[nodiscard]] bool operator==(const Function &other) const noexcept;
};

} // namespace nicole

#endif