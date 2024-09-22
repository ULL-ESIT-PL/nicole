#include "../../inc/visitors/codeGeneration.h"

#include "../../inc/lexicalAnalysis/type.h"
#include "../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../inc/parsingAnalysis/ast/conditionals/nodeIfStatement.h"
#include "../../inc/parsingAnalysis/ast/declaration/constDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varReassignment.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralBool.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralChar.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralDouble.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralInt.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeWhileStatement.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeBinaryOp.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeIncrement.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeUnaryOp.h"
#include "../../inc/parsingAnalysis/ast/statements/statement.h"
#include "../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "../../inc/parsingAnalysis/ast/utils/nodePrint.h"
#include "../../inc/parsingAnalysis/parsingAlgorithms/tree.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>
#include <string>

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeLiteralBool *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralChar *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralDouble *node) const {
  return llvm::ConstantFP::get(*context_, llvm::APFloat(node->value()));
}

llvm::Value *CodeGeneration::visit(const NodeLiteralInt *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_),
                                node->value(), true);
}

llvm::Value *CodeGeneration::visit(const NodeLiteralString *node) const {
  llvm::Constant *strConst = llvm::ConstantDataArray::getString(
      *context_, node->value(), /*AddNull=*/true);

  llvm::Value *globalString{
      builder_.CreateGlobalString(llvm::StringRef{node->value()}, "str", 0U)};
  // Obtener el puntero al string global
  llvm::Value *globalStrPtr = builder_.CreatePointerCast(
      globalString, llvm::PointerType::getUnqual(strConst->getType()));

  return globalStrPtr;
}

llvm::Value *CodeGeneration::visit(const NodeBinaryOp *node) const {
  const Node *left{node->left()};
  const Node *right{node->right()};

  llvm::Value *leftEvaluated{left->accept(this)};
  llvm::Value *rightEvaluated{right->accept(this)};
  if (!leftEvaluated || !rightEvaluated) {
    return nullptr;
  }
  // Convertir tipos si son diferentes (int a double)
  llvm::Type *leftType = leftEvaluated->getType();
  llvm::Type *rightType = rightEvaluated->getType();

  // Si left es int y right es double, convierte left a double
  if (leftType->isIntegerTy() && rightType->isFloatingPointTy()) {
    leftEvaluated =
        builder_.CreateSIToFP(leftEvaluated, rightType, "intToDouble");
  }
  // Si left es double y right es int, convierte right a double
  else if (leftType->isFloatingPointTy() && rightType->isIntegerTy()) {
    rightEvaluated =
        builder_.CreateSIToFP(rightEvaluated, leftType, "intToDouble");
  }

  // Asegúrate de que ambos operandos tengan el mismo tipo ahora (ambos double o
  // ambos int)
  if (leftEvaluated->getType() != rightEvaluated->getType()) {
    return nullptr; // Error: tipos no compatibles
  }
  // Realiza la operación basada en el tipo del operador
  switch (node->op()) {
  case TokenType::OPERATOR_ADD:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFAdd(leftEvaluated, rightEvaluated, "addtmp");
    } else {
      return builder_.CreateAdd(leftEvaluated, rightEvaluated, "addtmp");
    }
  case TokenType::OPERATOR_SUB:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFSub(leftEvaluated, rightEvaluated, "subtmp");
    } else {
      return builder_.CreateSub(leftEvaluated, rightEvaluated, "subtmp");
    }
  case TokenType::OPERATOR_MULT:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFMul(leftEvaluated, rightEvaluated, "multmp");
    } else {
      return builder_.CreateMul(leftEvaluated, rightEvaluated, "multmp");
    }
  case TokenType::OPERATOR_DIV:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFDiv(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder_.CreateSDiv(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::OPERATOR_MODULE:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      llvm::report_fatal_error(
          "Cannot calculate the module with decimal numbers");
    } else {
      return builder_.CreateURem(leftEvaluated, rightEvaluated, "modtmp");
    }
  case TokenType::OR:
    if (leftEvaluated->getType()->isIntegerTy()) {
      return builder_.CreateOr(leftEvaluated, rightEvaluated, "ormp");
    }
  case TokenType::AND:
    if (leftEvaluated->getType()->isIntegerTy()) {
      return builder_.CreateAnd(leftEvaluated, rightEvaluated, "andmp");
    }
  case TokenType::EQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOEQ(leftEvaluated, rightEvaluated, "equalmp");
    } else {
      return builder_.CreateICmpEQ(leftEvaluated, rightEvaluated, "equaltmp");
    }
  case TokenType::NOTEQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpONE(leftEvaluated, rightEvaluated,
                                    "notequalmp");
    } else {
      return builder_.CreateICmpNE(leftEvaluated, rightEvaluated,
                                   "notequaltmp");
    }
  case TokenType::OPERATOR_SMALLER:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOLT(leftEvaluated, rightEvaluated, "smallermp");
    } else {
      return builder_.CreateICmpSLT(leftEvaluated, rightEvaluated,
                                    "smallertmp");
    }
  case TokenType::SMALLEREQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOLE(leftEvaluated, rightEvaluated,
                                    "smallerequalmp");
    } else {
      return builder_.CreateICmpSLE(leftEvaluated, rightEvaluated,
                                    "smallerequaltmp");
    }
  case TokenType::OPERATOR_GREATER:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOGT(leftEvaluated, rightEvaluated, "greatermp");
    } else {
      return builder_.CreateICmpSGT(leftEvaluated, rightEvaluated,
                                    "greatertmp");
    }
  case TokenType::BIGGEREQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOGE(leftEvaluated, rightEvaluated,
                                    "biggerequalmp");
    } else {
      return builder_.CreateICmpSGE(leftEvaluated, rightEvaluated,
                                    "biggerequaltmp");
    }
  default:
    return nullptr;
  }
}

llvm::Value *CodeGeneration::visit(const NodeUnaryOp *node) const {
  const Node *expression{node->expression()};

  llvm::Value *expressionEvaluated{expression->accept(this)};
  if (!expressionEvaluated) {
    return nullptr;
  }
  llvm::Type *expressionType = expressionEvaluated->getType();
  switch (node->op()) {
  case TokenType::OPERATOR_NOT: {
    if (expressionType->isIntegerTy(1)) {
      return builder_.CreateNot(expressionEvaluated, "notTemp");
    } else {
      llvm::report_fatal_error("Can only use not operator with booleans");
    }
  }
  case TokenType::OPERATOR_SUB: {
    if (expressionType->isIntegerTy()) {
      return builder_.CreateNeg(expressionEvaluated, "arithNegTemp");
    } else if (expressionType->isDoubleTy()) {
      return builder_.CreateFNeg(expressionEvaluated, "arithNegTemp");
    } else {
      llvm::report_fatal_error(
          "Can only use arithmetic negation operator with int or double");
    }
  }
  default:
    llvm::llvm_unreachable_internal("Operator not supported");
  }
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeIncrement *node) const {
  const Node *expression{node->expression()};
  if (expression->type() != NodeType::CALL_VAR) {
    llvm::report_fatal_error(
        "Can only use ++ operator with variables of type int");
  }
  const NodeVariableCall *castedExpression{
      dynamic_cast<const NodeVariableCall *>(expression)};
  llvm::Value *expressionEvaluated{expression->accept(this)};
  if (!expressionEvaluated) {
    return nullptr;
  }
  llvm::Type *expressionType = expressionEvaluated->getType();
  if (!expressionType->isIntegerTy(32)) {
    llvm::report_fatal_error(
        "Can only use ++ operator with variables of type int");
  }
  llvm::Value *one{
      llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 1)};
  switch (node->op()) {
  case TokenType::INCREMENT: {
    llvm::Value *newValue{
        builder_.CreateAdd(expressionEvaluated, one, "increTemp")};
    llvm::AllocaInst *varAddress{
        castedExpression->table()->variableAddress(castedExpression->id())};
    builder_.CreateStore(newValue, varAddress);
    castedExpression->table()->setVariable(castedExpression->id(), newValue);
    return nullptr;
  }
  case TokenType::DECREMENT: {
    llvm::Value *newValue{
        builder_.CreateSub(expressionEvaluated, one, "decreTemp")};
    llvm::AllocaInst *varAddress{
        castedExpression->table()->variableAddress(castedExpression->id())};
    builder_.CreateStore(newValue, varAddress);
    castedExpression->table()->setVariable(castedExpression->id(), newValue);
    return nullptr;
  }
  default:
    llvm::llvm_unreachable_internal("Operator not supported");
  }
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStatement *node) const {
  return node->expression()->accept(this);
}

llvm::Value *CodeGeneration::visit(const NodeVariableDeclaration *node) const {
  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable

  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder_.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder_.CreateStore(value, alloca);
  const GenericType *varType{node->varType()};
  node->table()->addVariable(node->id(), varType, value, alloca);
  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeConstDeclaration *node) const {
  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable

  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder_.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder_.CreateStore(value, alloca);
  const GenericType *varType{node->varType()};
  node->table()->addVariable(node->id(), varType, value, alloca, true);
  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeVariableCall *node) const {
  std::cout << "---------\n" << *node->table() << std::flush;
  // return node->table()->variableValue(node->id());
  return builder_.CreateLoad(
      node->table()->variableValue(node->id())->getType(),
      node->table()->variableAddress(node->id()), node->id());
}

llvm::Value *CodeGeneration::visit(const NodeVariableReassignment *node) const {
  llvm::AllocaInst *varAddress{node->table()->variableAddress(node->id())};
  llvm::Value *newValue{node->expression()->accept(this)};
  builder_.CreateStore(newValue, varAddress);
  node->table()->setVariable(node->id(), newValue);

  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeIfStatement *node) const {
  llvm::Value *condition = node->condition()->accept(this);
  if (!condition) {
    return nullptr;
  }

  llvm::Function *TheFunction = builder_.GetInsertBlock()->getParent();
  llvm::BasicBlock *ThenBB =
      llvm::BasicBlock::Create(*context_, "then", TheFunction);
  llvm::BasicBlock *ElseBB = nullptr;
  llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*context_, "ifcont");

  if (node->hasElse()) {
    // Crear la instrucción condicional con 'then' y 'else'
    ElseBB = llvm::BasicBlock::Create(*context_, "else");
    builder_.CreateCondBr(condition, ThenBB, ElseBB);
  } else {
    // Crear la instrucción condicional con solo 'then'
    builder_.CreateCondBr(condition, ThenBB, MergeBB);
  }

  // Insertar el bloque 'then'
  builder_.SetInsertPoint(ThenBB);
  node->body()->accept(this); // Ejecutar el cuerpo del 'then'
  builder_.CreateBr(MergeBB); // Unir con MergeBB

  if (ElseBB) {
    // Insertar el bloque 'else'
    TheFunction->insert(TheFunction->end(), ElseBB);
    builder_.SetInsertPoint(ElseBB);
    node->elseBody()->accept(this); // Ejecutar el cuerpo del 'else'
    builder_.CreateBr(MergeBB);     // Unir con MergeBB
  }

  // Insertar el bloque 'merge'
  TheFunction->insert(TheFunction->end(), MergeBB);
  builder_.SetInsertPoint(MergeBB);
  // No devolver valor ya que el 'if' solo controla el flujo
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeWhileStatement *node) const {
  // Crear los bloques básicos: condición, cuerpo y finalización del while
  llvm::Function *TheFunction = builder_.GetInsertBlock()->getParent();

  llvm::BasicBlock *CondBB =
      llvm::BasicBlock::Create(*context_, "while.cond", TheFunction);
  llvm::BasicBlock *BodyBB = llvm::BasicBlock::Create(
      *context_, "while.body", TheFunction); // si pongo Thefunction luego no me
                                             // hace falta hacer un insert
  llvm::BasicBlock *EndBB = llvm::BasicBlock::Create(*context_, "while.end");

  // Saltar inmediatamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de condición
  builder_.SetInsertPoint(CondBB);

  // Evaluar la condición
  llvm::Value *condition = node->condition()->accept(this);
  if (!condition) {
    return nullptr; // Error al generar la condición
  }

  // Crear una instrucción condicional para decidir si continuar con el bucle o
  // terminar
  builder_.CreateCondBr(condition, BodyBB, EndBB);

  // Insertar el bloque del cuerpo del bucle
  builder_.SetInsertPoint(BodyBB);

  // Generar el cuerpo del bucle
  node->body()->accept(this);

  // Al finalizar el cuerpo, saltar nuevamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de finalización (salida del bucle)
  TheFunction->insert(TheFunction->end(), EndBB);
  builder_.SetInsertPoint(EndBB);

  // No retorna ningún valor
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStatementList *node) const {
  llvm::Value *lastValue{nullptr};
  for (const auto &statement : *node) {
    llvm::Value *value{statement->accept(this)};
    if (statement->expression()->type() == NodeType::VAR_DECL ||
        statement->expression()->type() == NodeType::CONST_DECL ||
        statement->expression()->type() == NodeType::VAR_REG ||
        statement->expression()->type() == NodeType::INCREMENT ||
        statement->expression()->type() == NodeType::IF ||
        statement->expression()->type() == NodeType::WHILE ||
        statement->expression()->type() == NodeType::PRINT) {
      // std::cout << "SKIPPED->>>"
      //          << nodeTypeToString(statement->expression()->type()) + "\n"
      //          << std::flush;
      continue;
    }
    if (!value) {
      return nullptr;
    }
    std::cout << "NOT SKIPPED->>>"
              << nodeTypeToString(statement->expression()->type()) + "\n"
              << std::flush;
    lastValue = value;
  }

  return lastValue;
}

llvm::Value *CodeGeneration::visit(const NodePrint *node) const {
   std::string result;
    auto value = node->expression()->accept(this);
    if (auto constantInt = llvm::dyn_cast<llvm::ConstantInt>(value)) {
        // Es un entero (puede ser bool o int)
        if (constantInt->getType()->isIntegerTy(1)) {
            result = constantInt->isZero() ? "false" : "true"; // Es un bool
        } else {
            result = std::to_string(constantInt->getSExtValue()); // Es un entero
        }
    } else if (auto constantFP = llvm::dyn_cast<llvm::ConstantFP>(value)) {
        // Es un valor de punto flotante (double)
        result = std::to_string(constantFP->getValueAPF().convertToDouble());
    } else if (auto globalString = llvm::dyn_cast<llvm::GlobalVariable>(value)) {
        if (auto initializer = llvm::dyn_cast<llvm::ConstantDataArray>(globalString->getInitializer())) {
            // Es un string
            if (initializer->isString()) {
                result = initializer->getAsString().str();
                result = result.substr(1, result.size() - 3);
            }
        }
    } else if (auto constantStr = llvm::dyn_cast<llvm::ConstantDataArray>(value)) {
        // Caso para obtener directamente una constante de tipo string
        if (constantStr->isString()) {
            result = constantStr->getAsString().str();
            result = result.substr(1, result.size() - 1);
        }
    }

    // Si no es ninguno de los anteriores, puedes usar la impresión de LLVM para inspeccionar el valor
    if (result.empty()) {
        std::string rawString;
        llvm::raw_string_ostream rso(rawString);
        value->print(rso);  // Imprime el valor en formato IR a un string
        result = rso.str(); // Asigna el valor impreso como resultado
    }
  std::string strToPrint{result};
  // Verificar si la función printf ya está en el módulo
  llvm::Function *printfFunc = module_->getFunction("printf");
  if (!printfFunc) {
    // Si printf no existe, se crea su declaración
    llvm::FunctionType *printfType = llvm::FunctionType::get(
        llvm::IntegerType::getInt32Ty(*context_), // printf retorna int
        llvm::PointerType::getUnqual(
            llvm::Type::getInt8Ty(*context_)), // primer arg: char*
        true // Es variádica (acepta múltiples args)
    );
    printfFunc = llvm::Function::Create(
        printfType, llvm::Function::ExternalLinkage, "printf", module_);
  }

  // Convertir la cadena std::string a una constante global de LLVM
  llvm::Value *formatStr = builder_.CreateGlobalStringPtr(strToPrint, "fmt");

  // Llamar a printf con la cadena de formato
  std::vector<llvm::Value *> args;
  args.push_back(formatStr); // Primer argumento: el puntero a la cadena (char*)

  // Crear la llamada a printf
  llvm::Value *call = builder_.CreateCall(printfFunc, args, "calltmp");

  // return call; // Retornar la llamada creada
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const Tree *node) const {
  llvm::Value *val{node->root()->accept(this)};
  return builder_.CreateRetVoid();
}

llvm::Value *CodeGeneration::generate(const Tree *tr) const {
  std::cout << "Generate--> " << builder_.GetInsertBlock()->getName().str()
            << "\n";
  auto val{visit(tr)};
  std::cout << "Generate--> " << builder_.GetInsertBlock()->getName().str()
            << "\n";
  return val;
}
} // namespace nicole
