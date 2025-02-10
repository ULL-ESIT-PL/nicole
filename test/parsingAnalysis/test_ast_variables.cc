#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_AUTO_DECL class methods", "[AST_AUTO_DECL]") {
  auto astBool = std::make_shared<AST_BOOL>(true);

  AST_AUTO_DECL astAutoDecl{"myVar", astBool, true};

  REQUIRE(astAutoDecl.id() == "myVar");
  REQUIRE(astAutoDecl.isConst() == true);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astAutoDecl.value())->value() ==
          true);
}

TEST_CASE("AST_VAR_TYPED_DECL class methods", "[AST_VAR_TYPED_DECL]") {
  auto astBool = std::make_shared<AST_BOOL>(true);

  AST_VAR_TYPED_DECL astLetDecl{"myLet", "bool", astBool, true};

  REQUIRE(astLetDecl.id() == "myLet");
  REQUIRE(astLetDecl.varType() == "bool");
  REQUIRE(astLetDecl.isConst() == true);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astLetDecl.value())->value() ==
          true);
}

TEST_CASE("AST_VAR_CALL class methods", "[AST_VAR_CALL]") {
  AST_VAR_CALL astVarCall{"myVariable"};

  REQUIRE(astVarCall.id() == "myVariable");
}
