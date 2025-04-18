#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_IMPORT class methods", "[AST_IMPORT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  AST_IMPORT astImport{0, SourceLocation{token, token}, "/path/to/file"};

  REQUIRE(astImport.path() == std::filesystem::path{"/path/to/file"});
}

bool areASTStatementsEqual(const std::shared_ptr<AST_STATEMENT> &lhs,
                           const std::shared_ptr<AST_STATEMENT> &rhs) {
  // Verifica si ambos punteros son nulos
  if (!lhs && !rhs)
    return true;
  // Verifica si solo uno de los punteros es nulo
  if (!lhs || !rhs)
    return false;
  // Compara los tipos de las expresiones
  if (lhs->expression()->type() != rhs->expression()->type())
    return false;
  // Realiza comparaciones específicas según el tipo de expresión
  // Por ejemplo, si es AST_BOOL, compara los valores booleanos
  if (lhs->expression()->type() == AST_TYPE::BOOL) {
    auto lhsBool = std::dynamic_pointer_cast<AST_BOOL>(lhs->expression());
    auto rhsBool = std::dynamic_pointer_cast<AST_BOOL>(rhs->expression());
    return lhsBool && rhsBool && lhsBool->value() == rhsBool->value();
  }
  return false; // Por defecto, se considera que no son iguales
}

TEST_CASE("AST_PRINT class methods", "[AST_PRINT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto astBool1 = *Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto astBool2 = *Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, false));
  auto astComma = std::vector<std::shared_ptr<AST>>{astBool1, astBool2};

  AST_PRINT astPrint{0, SourceLocation{token, token}, astComma};

  REQUIRE(astPrint.values() == astComma);
  REQUIRE(astPrint.values()[0] == astBool1);
  REQUIRE(astPrint.values()[1] == astBool2);
}
