#include "../../inc/parsingAnalysis/ast/functions/ast_funcCall.h"
#include "../../inc/parsingAnalysis/ast/functions/ast_funcDecl.h"
#include "../../inc/parsingAnalysis/ast/functions/ast_return.h"
#include "../../inc/parsingAnalysis/ast/functions/parameters.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("Parameters class methods", "[Parameters]") {
  Parameters params({{"x", "int"}, {"y", "float"}});

  REQUIRE(params.size() == 2);
  REQUIRE(params.params()[0].first == "x");
  REQUIRE(params.params()[0].second == "int");
  REQUIRE(params.params()[1].first == "y");
  REQUIRE(params.params()[1].second == "float");

  Parameters otherParams({{"x", "int"}, {"y", "float"}});
  REQUIRE(params == otherParams);

  Parameters differentParams({{"a", "int"}});
  REQUIRE_FALSE(params == differentParams);
}

TEST_CASE("AST_FUNC_CALL class methods", "[AST_FUNC_CALL]") {
  auto param1 = std::make_shared<AST_BOOL>(true);
  auto param2 = std::make_shared<AST_BOOL>(false);
  AST_FUNC_CALL funcCall("myFunction", {param1, param2});

  REQUIRE(funcCall.id() == "myFunction");
  REQUIRE(funcCall.parameters().size() == 2);
  REQUIRE(funcCall.parameters()[0] == param1);
  REQUIRE(funcCall.parameters()[1] == param2);
}

TEST_CASE("AST_FUNC_DECL class methods", "[AST_FUNC_DECL]") {
  Parameters params({{"x", "int"}, {"y", "float"}});
  auto statement =
      std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(true));
  auto body = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_FUNC_DECL funcDecl("myFunction", params, "void", body, true);

  REQUIRE(funcDecl.id() == "myFunction");
  REQUIRE(funcDecl.parameters() == params);
  REQUIRE(funcDecl.returnType() == "void");
  REQUIRE(funcDecl.body() == body);
  REQUIRE(funcDecl.isMehtod() == true);
}

TEST_CASE("AST_RETURN class methods", "[AST_RETURN]") {
  auto returnValue = std::make_shared<AST_BOOL>(true);
  AST_RETURN astReturn{returnValue};

  REQUIRE(astReturn.expression() == returnValue);
  REQUIRE(
      std::dynamic_pointer_cast<AST_BOOL>(astReturn.expression())->value() ==
      true);
}
