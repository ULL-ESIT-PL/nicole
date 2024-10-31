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
  auto floatType{std::make_shared<FloatType>()};
  table_[floatType->name()] = floatType;
  auto intType{std::make_shared<IntType>()};
  table_[intType->name()] = intType;
  auto stringType{std::make_shared<StringType>()};
  table_[stringType->name()] = stringType;
  auto voidType{std::make_shared<VoidType>()};
  table_[voidType->name()] = voidType;
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

std::shared_ptr<GenericType> TypeTable::keyFromLLVMType(llvm::Type *llvmType, llvm::LLVMContext &context) const {
  // Iteramos sobre todos los tipos en la tabla
  for (const auto &entry : table_) {
    const std::string &typeName = entry.first;
    const std::shared_ptr<GenericType> &genType = entry.second;
    llvm::Type *genLLVMType = genType->type(&context);

    if (areTypesEquivalent(llvmType, genLLVMType)) {
      return genType;
    }
  }
  // Si no se encuentra, devolvemos una cadena vacía o manejamos el error según sea necesario
  llvm::report_fatal_error("Cannot find type using llvm types");
}

bool TypeTable::areTypesEquivalent(llvm::Type *type1, llvm::Type *type2) const {
  if (type1 == type2) {
    return true;
  }

  // Comparamos los IDs de tipo
  if (type1->getTypeID() != type2->getTypeID()) {
    return false;
  }

  switch (type1->getTypeID()) {
  case llvm::Type::IntegerTyID:
    // Comparación para tipos enteros
    return type1->getIntegerBitWidth() == type2->getIntegerBitWidth();

  case llvm::Type::PointerTyID:
    // Dado que los punteros son opacos, solo podemos comparar las propiedades
    // que conocemos Podemos verificar el espacio de direcciones
    return type1->getPointerAddressSpace() == type2->getPointerAddressSpace();

  case llvm::Type::StructTyID: {
    // Para tipos de struct, comparamos los nombres
    llvm::StructType *struct1 = llvm::cast<llvm::StructType>(type1);
    llvm::StructType *struct2 = llvm::cast<llvm::StructType>(type2);
    return struct1->getName() == struct2->getName();
  }

  case llvm::Type::FloatTyID:
  case llvm::Type::DoubleTyID:
    // Para tipos de punto flotante, los IDs de tipo son suficientes
    return true;

    // Agrega más casos si es necesario

  default:
    return false;
  }
}

} // namespace nicole