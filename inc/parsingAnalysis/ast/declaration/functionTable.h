#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>

#include "../../types/genericType.h"
#include "../node.h"
#include "llvm/IR/Function.h"

namespace nicole {

class FunctionTable final {
private:
  std::unordered_map<std::string,
                     std::tuple<llvm::Function *, const GenericType *>>
      table_{};

public:
  FunctionTable() {};

  auto begin() const { return table_.begin(); }

  auto end() const { return table_.end(); }

  bool hasFunction(const std::string &id);

  void addFunction(const std::string &id, const GenericType *idType,
                   llvm::Function *fun);

  llvm::Function *function(const std::string &id);

  const GenericType *functionType(const std::string &id);

  friend std::ostream &operator<<(std::ostream &os, const FunctionTable &scope);
};

} // namespace nicole

#endif