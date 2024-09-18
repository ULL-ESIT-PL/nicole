#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>

#include "../../types/genericType.h"
#include "../node.h"

namespace nicole {

class VariableTable final {
 private:
  std::shared_ptr<VariableTable> father_;
  std::unordered_map<
      std::string, std::tuple<std::unique_ptr<GenericType>,
                              std::pair<llvm::Value*, llvm::AllocaInst*>, bool>>
      table_{};

 public:
  VariableTable(std::shared_ptr<VariableTable> father = nullptr)
      : father_{father} {};

  std::shared_ptr<VariableTable> father() { return father_; }

  auto begin() const { return table_.begin(); }

  auto end() const { return table_.end(); }

  bool hasVariable(const std::string& id);

  void addVariable(const std::string& id, std::unique_ptr<GenericType> idType,
                   llvm::Value* value, llvm::AllocaInst* alloca, const bool = false);

  void setVariable(const std::string& id, llvm::Value* value);

  llvm::Value* variableValue(const std::string& id);

  llvm::AllocaInst* variableAdress(const std::string& id);

  GenericType* variableType(const std::string& id);

  friend std::ostream& operator<<(std::ostream& os, const VariableTable& scope);
};

}  // namespace nicole

#endif