#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_ASSIGNMENT class methods", "[AST_ASSIGNMENT]") {
  auto astBool = std::make_shared<AST_BOOL>(true);
  auto left = std::make_shared<AST_BOOL>(true);
  AST_ASSIGNMENT astAssignment{left, astBool};

  REQUIRE(astAssignment.left() == left);
  REQUIRE(astAssignment.value() == astBool);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astAssignment.value())->value() ==
          true);
}

TEST_CASE("AST_SELF_ASSIGNMENT class methods", "[AST_SELF_ASSIGNMENT]") {
  const Token op{TokenType::SELF_ADD, "+=", Location{"archivo.cpp", 10, 5}};
  auto astBool = std::make_shared<AST_BOOL>(true);
  auto left = std::make_shared<AST_BOOL>(true);
  AST_SELF_ASSIGNMENT astSelfAdd{op, left, astBool};

  REQUIRE(astSelfAdd.op().raw() == "+=");
  REQUIRE(astSelfAdd.left() == left);
  REQUIRE(astSelfAdd.value() == astBool);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astSelfAdd.value())->value() ==
          true);
}