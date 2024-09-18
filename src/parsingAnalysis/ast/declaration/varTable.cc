#include "../../../../inc/parsingAnalysis/ast/declaration/varTable.h"

namespace nicole {

bool VariableTable::hasVariable(const std::string& id) {
  if (table_.count(id)) return true;
  if (father_) return father_->hasVariable(id);
  return false;
}

void VariableTable::addVariable(const std::string& id,
                                std::unique_ptr<GenericType> idType,
                                llvm::Value* value, llvm::AllocaInst* alloca,
                                const bool isConst) {
  if (!hasVariable(id)) {
    table_[id] =
        std::make_tuple(std::move(idType), std::pair{value, alloca}, isConst);
    return;
  }
  const std::string strErr{"The variable " + id + " already exist"};
  llvm::report_fatal_error(strErr.c_str());
}

void VariableTable::setVariable(const std::string& id, llvm::Value* value) {
  if (table_.count(id)) {
    if (std::get<2>(table_[id])) {
      const std::string strErr{"Cannot modify the variable " + id +
                               " due being const"};
      llvm::report_fatal_error(strErr.c_str());
    }
    std::get<1>(table_[id]).first = value;
  } else if (father_) {
    father_->setVariable(id, value);
  } else {
    const std::string strErr{"The variable " + id + " does not exist"};
    llvm::report_fatal_error(strErr.c_str());
  }
}

llvm::Value* VariableTable::variableValue(const std::string& id) {
  if (table_.count(id)) return std::get<1>(table_.at(id)).first;
  if (father_) return father_->variableValue(id);
  const std::string strErr{"The variable " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

llvm::AllocaInst* VariableTable::variableAdress(const std::string& id) {
  if (table_.count(id)) return std::get<1>(table_.at(id)).second;
  if (father_) return father_->variableAdress(id);
  const std::string strErr{"The variable " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

GenericType* VariableTable::variableType(const std::string& id) {
  if (table_.count(id)) return std::get<0>(table_.at(id)).get();
  if (father_) return father_->variableType(id);
  const std::string strErr{"The variable " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

std::ostream& operator<<(std::ostream& os, const VariableTable& scope) {
  if (scope.father_) {
    os << *scope.father_;
  }
  os << "Table size: " + std::to_string(scope.table_.size()) + "\n";
  for (auto&& variable : scope) {
    os << "Var: " << variable.first << "\n";
  }
  return os;
}

}  // namespace nicole