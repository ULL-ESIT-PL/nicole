#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_UNARY class methods", "[AST_UNARY]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::DECREMENT, "--", loc};
  auto astInt = std::make_shared<AST_INT>(42);
  AST_UNARY astDecrement{token, astInt};

  REQUIRE(astDecrement.op().raw() == "--");
  REQUIRE(astDecrement.value() == astInt);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astDecrement.value())->value() ==
          42);
}