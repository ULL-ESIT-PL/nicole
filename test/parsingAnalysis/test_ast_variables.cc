#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_constDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_letDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_varCall.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_AUTO_DECL class methods", "[AST_AUTO_DECL]") {
  auto astBool = std::make_shared<AST_BOOL>(true);

  AST_AUTO_DECL astAutoDecl{"myVar", astBool};

  REQUIRE(astAutoDecl.id() == "myVar");
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astAutoDecl.value())->value() ==
          true);
}

TEST_CASE("AST_CONST_DECL class methods", "[AST_CONST_DECL]") {
  auto astBool = std::make_shared<AST_BOOL>(false);

  AST_CONST_DECL astConstDecl{"myConst", "bool", astBool};

  REQUIRE(astConstDecl.id() == "myConst");
  REQUIRE(astConstDecl.type() == "bool");
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astConstDecl.value())->value() ==
          false);
}

TEST_CASE("AST_LET_DECL class methods", "[AST_LET_DECL]") {
  auto astBool = std::make_shared<AST_BOOL>(true);

  AST_LET_DECL astLetDecl{"myLet", "bool", astBool};

  REQUIRE(astLetDecl.id() == "myLet");
  REQUIRE(astLetDecl.type() == "bool");
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astLetDecl.value())->value() ==
          true);
}

TEST_CASE("AST_VAR_CALL class methods", "[AST_VAR_CALL]") {
  AST_VAR_CALL astVarCall{"myVariable"};

  REQUIRE(astVarCall.id() == "myVariable");
}
