#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include "boolType.h"
#include "charType.h"
#include "floatType.h"
#include "intType.h"
#include "stringType.h"
#include "userType.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace nicole {

class TypeTable {
private:
  mutable std::unordered_map<std::string, std::shared_ptr<GenericType>> table_;

  void addPrimitives() const;

public:
  TypeTable() { addPrimitives(); }

  bool hasType(const std::string &name) const;

  void addType(std::shared_ptr<GenericType> type) const;

  std::shared_ptr<GenericType> type(const std::string &name) const;
};

} // namespace nicole

#endif