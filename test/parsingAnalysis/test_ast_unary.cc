#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_int.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_decrement.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_increment.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_neg.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_not.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_DECREMENT class methods", "[AST_DECREMENT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::DECREMENT, "--", loc};
  auto astInt = std::make_shared<AST_INT>(42);
  AST_DECREMENT astDecrement{token, astInt};

  REQUIRE(astDecrement.op().raw() == "--");
  REQUIRE(astDecrement.value() == astInt);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astDecrement.value())->value() ==
          42);
}

TEST_CASE("AST_INCREMENT class methods", "[AST_INCREMENT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::INCREMENT, "++", loc};
  auto astInt = std::make_shared<AST_INT>(42);
  AST_INCREMENT astIncrement{token, astInt};

  REQUIRE(astIncrement.op().raw() == "++");
  REQUIRE(astIncrement.value() == astInt);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astIncrement.value())->value() ==
          42);
}

TEST_CASE("AST_NEG class methods", "[AST_NEG]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_SUB, "-", loc};
  auto astInt = std::make_shared<AST_INT>(42);
  AST_NEG astNeg{token, astInt};

  REQUIRE(astNeg.op().raw() == "-");
  REQUIRE(astNeg.value() == astInt);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astNeg.value())->value() == 42);
}

TEST_CASE("AST_NOT class methods", "[AST_NOT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_NOT, "!", loc};
  auto astBool = std::make_shared<AST_BOOL>(true);
  AST_NOT astNot{token, astBool};

  REQUIRE(astNot.op().raw() == "!");
  REQUIRE(astNot.value() == astBool);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astNot.value())->value() == true);
}
