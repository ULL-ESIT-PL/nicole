#include "../../inc/parsingAnalysis/ast/assignments/ast_assignment.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfAdd.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfDiv.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfMult.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfSub.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
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

TEST_CASE("AST_SELF_ADD class methods", "[AST_SELF_ADD]") {
  auto astBool = std::make_shared<AST_BOOL>(true);
  auto left = std::make_shared<AST_BOOL>(true);
  AST_SELF_ADD astSelfAdd{left, astBool};

  REQUIRE(astSelfAdd.left() == left);
  REQUIRE(astSelfAdd.value() == astBool);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astSelfAdd.value())->value() ==
          true);
}

TEST_CASE("AST_SELF_SUB class methods", "[AST_SELF_SUB]") {
  auto astBool = std::make_shared<AST_BOOL>(false);
  auto left = std::make_shared<AST_BOOL>(true);
  AST_SELF_SUB astSelfSub{left, astBool};

  REQUIRE(astSelfSub.left() == left);
  REQUIRE(astSelfSub.value() == astBool);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astSelfSub.value())->value() ==
          false);
}

TEST_CASE("AST_SELF_MULT class methods", "[AST_SELF_MULT]") {
  auto astBool = std::make_shared<AST_BOOL>(true);
  auto left = std::make_shared<AST_BOOL>(true);
  AST_SELF_MULT astSelfMult{left, astBool};

  REQUIRE(astSelfMult.left() == left);
  REQUIRE(astSelfMult.value() == astBool);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astSelfMult.value())->value() ==
          true);
}

TEST_CASE("AST_SELF_DIV class methods", "[AST_SELF_DIV]") {
  auto astBool = std::make_shared<AST_BOOL>(false);
  auto left = std::make_shared<AST_BOOL>(true);
  AST_SELF_DIV astSelfDiv{left, astBool};

  REQUIRE(astSelfDiv.left() == left);
  REQUIRE(astSelfDiv.value() == astBool);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astSelfDiv.value())->value() ==
          false);
}
