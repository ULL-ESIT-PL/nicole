#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include <llvm/IR/Value.h>

#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>

#include "../types/genericType.h"

namespace nicole {

class VariableTable final {
 private:
  std::unique_ptr<VariableTable> father_;
  std::unordered_map<
      std::string, std::tuple<std::unique_ptr<GenericType>, llvm::Value*, const bool>>
      table_{};

 public:
  VariableTable(std::unique_ptr<VariableTable> father = nullptr)
      : father_{std::move(father)} {};

  VariableTable* father() const { return father_.get(); }

  auto begin() const { return table_.begin(); }

  auto end() const { return table_.end(); }
};

}  // namespace nicole

#endif