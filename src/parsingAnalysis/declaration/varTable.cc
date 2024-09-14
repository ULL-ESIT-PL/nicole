#include "../../../inc/parsingAnalysis/declaration/varTable.h"

namespace nicole {

bool VariableTable::hasVariable(const std::string& id) {
  if (table_.count(id)) return true;
  if (father_) return father_->hasVariable(id);
  return false;
}

void VariableTable::addVariable(const std::string& id,
                                std::unique_ptr<GenericType> idType,
                                std::unique_ptr<Node> value,
                                const bool isConst) {
  if (!hasVariable(id)) {
    table_[id] = std::make_tuple(std::move(idType), std::move(value), isConst);
  }
}

void VariableTable::setVariable(const std::string& id,
                                std::unique_ptr<Node> value) {
  if (table_.count(id)) {
    if (get<2>(table_[id])) {
      const std::string strErr{"Cannot modify the variable " + id +
                               " due being const"};
      llvm::report_fatal_error(strErr.c_str());
    }
    get<1>(table_[id]) = std::move(value);
  } else if (father_) {
    father_->setVariable(id, std::move(value));
  } else {
    const std::string strErr{"The variable " + id + " does not exist"};
    llvm::report_fatal_error(strErr.c_str());
  }
}

Node* VariableTable::variableValue(const std::string& id) {
  if (table_.count(id)) return get<1>(table_.at(id)).get();
  if (father_) return father_->variableValue(id);
  const std::string strErr{"The variable " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

GenericType* VariableTable::variableType(const std::string& id) {
  if (table_.count(id)) return get<0>(table_.at(id)).get();
  if (father_) return father_->variableType(id);
  const std::string strErr{"The variable " + id + " does not exist"};
  llvm::report_fatal_error(strErr.c_str());
}

std::ostream& operator<<(std::ostream& os, const VariableTable& scope) {
  if (scope.father_) {
    os << *scope.father_;
  }
  os << "Table:\n";
  for (auto&& variable : scope) {
    os << "Var: " << variable.first << "\n";
  }
  return os;
}

}  // namespace nicole