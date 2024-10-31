#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include "boolType.h"
#include "charType.h"
#include "floatType.h"
#include "genericType.h"
#include "intType.h"
#include "stringType.h"
#include "doubleType.h"
#include "voidType.h"
#include "userType.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace nicole {

// Table to store the types built-in and user tpes
class TypeTable {
private:
  mutable std::unordered_map<std::string, std::shared_ptr<GenericType>> table_;

  void addPrimitives() const;

public:
  TypeTable() { addPrimitives(); }

  bool hasType(const std::string &name) const;

  void addType(std::shared_ptr<GenericType> type) const;

  std::shared_ptr<GenericType> type(const std::string &name) const;

  std::shared_ptr<GenericType> keyFromLLVMType(llvm::Type *llvmType, llvm::LLVMContext &context) const;

  bool areTypesEquivalent(llvm::Type *type1, llvm::Type *type2) const;
};

} // namespace nicole

#endif