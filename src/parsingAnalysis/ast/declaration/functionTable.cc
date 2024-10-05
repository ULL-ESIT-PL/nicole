#include "../../../../inc/parsingAnalysis/ast/declaration/functionTable.h"
#include "llvm/IR/Function.h"

namespace nicole {

bool FunctionTable::hasFunction(const std::string &id) {
  if (table_.count(id))
    return true;
  if (auto fatherShared = father_.lock()) {
    return fatherShared->hasFunction(id);
  }
  return false;
}

void FunctionTable::addFunction(const std::string &id,
                                const GenericType *idType,
                                llvm::Function *fun) {
  if (!hasFunction(id)) {
    table_[id] = std::make_tuple(fun, idType);
    return;
  }
  const std::string strErr{"The Function " + id + " already exist"};
  llvm::report_fatal_error(strErr.c_str());
}

llvm::Function *FunctionTable::function(const std::string &id) {
  if (table_.count(id))
    return std::get<0>(table_.at(id));
  if (auto fatherShared = father_.lock())
    return fatherShared->function(id);
  const std::string strErr{"The function " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

const GenericType *FunctionTable::functionType(const std::string &id) {
  if (table_.count(id))
    return std::get<1>(table_.at(id));
  if (auto fatherShared = father_.lock())
    return fatherShared->functionType(id);
  const std::string strErr{"The function " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

std::ostream &operator<<(std::ostream &os, const FunctionTable &scope) {
  if (auto fatherShared = scope.father_.lock()) {
    os << *fatherShared;
  }
  os << "Table size: " + std::to_string(scope.table_.size()) + "\n";
  for (auto &&fun : scope) {
    os << "Fun: " << fun.first << "\n";
  }
  return os;
}

} // namespace nicole