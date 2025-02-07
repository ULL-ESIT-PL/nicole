#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_BINARY class methods", "[AST_BINARY]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto leftOperand = std::make_shared<AST_INT>(21);
  auto rightOperand = std::make_shared<AST_INT>(21);
  AST_BINARY astAdd{token, leftOperand, rightOperand};

  REQUIRE(astAdd.op().raw() == "+");
  REQUIRE(astAdd.left() == leftOperand);
  REQUIRE(astAdd.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astAdd.left())->value() == 21);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astAdd.right())->value() == 21);
}