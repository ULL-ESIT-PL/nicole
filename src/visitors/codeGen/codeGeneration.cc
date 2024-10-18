#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/statements/statement.h"
#include "../../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "../../../inc/parsingAnalysis/parsingAlgorithms/tree.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeStatement *node) const {
  return node->expression()->accept(this);
}

llvm::Value *CodeGeneration::visit(const NodeStatementList *node) const {
  llvm::Value *lastValue{nullptr};
  for (const auto &statement : *node) {
    llvm::Value *value{statement->accept(this)};
    if (/* statement->expression()->type() == NodeType::VAR_DECL ||
        statement->expression()->type() == NodeType::CONST_DECL ||
        statement->expression()->type() == NodeType::VAR_REG ||
        statement->expression()->type() == NodeType::IF ||
        statement->expression()->type() == NodeType::WHILE ||
        statement->expression()->type() == NodeType::FOR ||
        statement->expression()->type() == NodeType::STOP ||
        statement->expression()->type() == NodeType::PASS ||
        statement->expression()->type() == NodeType::STRUCT_DECL ||
        statement->expression()->type() == NodeType::FUN_DECL ||
        statement->expression()->type() == NodeType::PRINT ||
        statement->expression()->type() == NodeType::IMPORT */
        !value) {
      // std::cout << "SKIPPED->>>"
      //         << nodeTypeToString(statement->expression()->type()) + "\n"
      //       << std::flush;
      continue;
    }
    /*
    if (!value) {
      return nullptr;
    }
    */
    // std::cout << "NOT SKIPPED->>>"
    //       << nodeTypeToString(statement->expression()->type()) + "\n"
    //     << std::flush;
    lastValue = value;
  }

  return lastValue;
}

llvm::Value *CodeGeneration::visit(const Tree *node) const {
  llvm::Value *val{node->root()->accept(this)};
  /*
    llvm::StructType *pointType =
        llvm::StructType::getTypeByName(*context_, "point");
    llvm::Constant *xValue =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 10); // x = 10
    llvm::Constant *yValue =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 20); // y = 20

    // Crear una constante de estructura con los valores de `x` e `y`
    llvm::Constant *pointInstance =
        llvm::ConstantStruct::get(pointType, {xValue, yValue});

    // Crear una variable local para `point`
    llvm::AllocaInst *pointVar =
        builder_.CreateAlloca(pointType, nullptr, "pointVar");

    // Almacenar la instancia en `pointVar`
    builder_.CreateStore(pointInstance, pointVar);

    // Acceder al campo `y`
    // Primero, cargamos la estructura desde `pointVar`
    llvm::LoadInst *loadedPoint = builder_.CreateLoad(pointType, pointVar,
    "loadedPoint");

    // Extraer el campo `y`, que se supone es el índice 1 en la estructura
    llvm::Value *yField = builder_.CreateExtractValue(loadedPoint, 1, "yField");

    // Reasignar `y` a un nuevo valor (40)
    llvm::Constant *newYValue =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 40);

    // Crear una nueva instancia de la estructura con el nuevo valor de `y`
    llvm::Value *newPoint =
        llvm::ConstantStruct::get(pointType, {xValue, newYValue});

    // Almacenar la nueva estructura de vuelta en `pointVar`
    builder_.CreateStore(newPoint, pointVar);
    */
  return builder_.CreateRetVoid();
}

llvm::Value *CodeGeneration::generate(const Tree *tr) const {
  // std::cout << "Generate--> " << builder_.GetInsertBlock()->getName().str()
  //         << "\n";
  auto val{visit(tr)};
  // std::cout << "Generate--> " << builder_.GetInsertBlock()->getName().str()
  //       << "\n";
  return val;
}
} // namespace nicole
