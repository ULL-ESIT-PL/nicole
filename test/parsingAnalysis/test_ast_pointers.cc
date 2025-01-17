#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_delete.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_deref.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_new.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_ptr.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>

using namespace nicole;

TEST_CASE("AST_DELETE class methods", "[AST_DELETE]") {
  auto value = std::make_shared<AST_BOOL>(true);
  AST_DELETE astDelete{value};

  REQUIRE(astDelete.value() == value);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astDelete.value())->value() ==
          true);
}

TEST_CASE("AST_DEREF class methods", "[AST_DEREF]") {
  auto value = std::make_shared<AST_BOOL>(false);
  AST_DEREF astDeref{value};

  REQUIRE(astDeref.value() == value);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astDeref.value())->value() ==
          false);
}

TEST_CASE("AST_NEW class methods", "[AST_NEW]") {
  auto value = std::make_shared<AST_BOOL>(true);
  AST_NEW astNew{value};

  REQUIRE(astNew.value() == value);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astNew.value())->value() == true);
}

TEST_CASE("AST_PTR class methods", "[AST_PTR]") {
  auto value = std::make_shared<AST_BOOL>(false);
  AST_PTR astPtr{"myPtr", "bool", value};

  REQUIRE(astPtr.id() == "myPtr");
  REQUIRE(astPtr.valueType() == "bool");
  REQUIRE(astPtr.value() == value);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astPtr.value())->value() ==
          false);
}
