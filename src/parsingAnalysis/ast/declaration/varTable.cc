#include "../../../../inc/parsingAnalysis/ast/declaration/varTable.h"

namespace nicole {

bool VariableTable::hasVariable(const std::string &id) {
  if (table_.count(id))
    return true;
  if (auto fatherShared = father_.lock()) {
    return fatherShared->hasVariable(id);
  }
  return false;
}

void VariableTable::addVariable(const std::string &id,
                                const GenericType *idType, llvm::Value *value,
                                llvm::AllocaInst *alloca, const bool isConst) {
  if (!hasVariable(id)) {
    table_[id] =
        std::make_tuple(std::move(idType), std::pair{value, alloca}, isConst);
    return;
  }
  const std::string strErr{"The variable " + id + " already exist"};
  llvm::report_fatal_error(strErr.c_str());
}

void VariableTable::setVariable(const std::string &id, llvm::Value *value) {
  if (table_.count(id)) {
    if (std::get<2>(table_[id])) {
      const std::string strErr{"Cannot modify the variable " + id +
                               " due being const"};
      llvm::report_fatal_error(strErr.c_str());
    }
    std::get<1>(table_[id]).first = value;
  } else if (auto fatherShared = father_.lock()) {
    fatherShared->setVariable(id, value);
  } else {
    const std::string strErr{"The variable " + id + " does not exist"};
    llvm::report_fatal_error(strErr.c_str());
  }
}

llvm::Value *VariableTable::variableValue(const std::string &id) {
  if (table_.count(id))
    return std::get<1>(table_.at(id)).first;
  if (auto fatherShared = father_.lock())
    return fatherShared->variableValue(id);
  const std::string strErr{"The variable " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

llvm::AllocaInst *VariableTable::variableAddress(const std::string &id) {
  if (table_.count(id))
    return std::get<1>(table_.at(id)).second;
  if (auto fatherShared = father_.lock())
    return fatherShared->variableAddress(id);
  const std::string strErr{"The variable " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

const GenericType *VariableTable::variableType(const std::string &id) {
  if (table_.count(id))
    return std::get<0>(table_.at(id));
  if (auto fatherShared = father_.lock())
    return fatherShared->variableType(id);
  const std::string strErr{"The variable " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

std::ostream &operator<<(std::ostream &os, const VariableTable &scope) {
  if (auto fatherShared = scope.father_.lock()) {
    os << *fatherShared;
  }
  os << "Table size: " + std::to_string(scope.table_.size()) + "\n";
  for (auto &&variable : scope) {
    os << "Var: " << variable.first << "\n";
  }
  return os;
}

} // namespace nicole