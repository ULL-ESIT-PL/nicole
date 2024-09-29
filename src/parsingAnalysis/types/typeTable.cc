#include "../../../inc/parsingAnalysis/types/typeTable.h"
#include "llvm/Support/ErrorHandling.h"
#include <memory>

namespace nicole {

void TypeTable::addPrimitives() const {
  auto boolType{std::make_shared<BoolType>()};
  table_[boolType->name()] = boolType;
  auto charType{std::make_shared<CharType>()};
  table_[charType->name()] = charType;
  auto doubleType{std::make_shared<DoubleType>()};
  table_[doubleType->name()] = doubleType;
  auto intType{std::make_shared<IntType>()};
  table_[intType->name()] = intType;
  auto stringType{std::make_shared<StringType>()};
  table_[stringType->name()] = stringType;
}

bool TypeTable::hasType(const std::string &name) const {
  return table_.count(name);
}

void TypeTable::addType(std::shared_ptr<GenericType> type) const {
  table_[type->name()] = type;
}

std::shared_ptr<GenericType> TypeTable::type(const std::string &name) const {
  if (!hasType(name)) {
    const std::string errStr{"The type: " + name + " does not exist"};
    llvm::report_fatal_error(errStr.c_str());
  }
  
  return table_.at(name);
}

} // namespace nicole