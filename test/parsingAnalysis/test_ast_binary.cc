#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_int.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_add.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_and.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_bigger.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_biggerEqual.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_div.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_equal.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_module.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_mult.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_notEqual.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_or.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_smaller.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_smallerEqual.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_sub.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_ADD class methods", "[AST_ADD]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto leftOperand = std::make_shared<AST_INT>(21);
  auto rightOperand = std::make_shared<AST_INT>(21);
  AST_ADD astAdd{token, leftOperand, rightOperand};

  REQUIRE(astAdd.op().raw() == "+");
  REQUIRE(astAdd.left() == leftOperand);
  REQUIRE(astAdd.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astAdd.left())->value() == 21);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astAdd.right())->value() == 21);
}

TEST_CASE("AST_SUB class methods", "[AST_SUB]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_SUB, "-", loc};
  auto leftOperand = std::make_shared<AST_INT>(42);
  auto rightOperand = std::make_shared<AST_INT>(21);
  AST_SUB astSub{token, leftOperand, rightOperand};

  REQUIRE(astSub.op().raw() == "-");
  REQUIRE(astSub.left() == leftOperand);
  REQUIRE(astSub.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astSub.left())->value() == 42);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astSub.right())->value() == 21);
}

TEST_CASE("AST_MULT class methods", "[AST_MULT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_MULT, "*", loc};
  auto leftOperand = std::make_shared<AST_INT>(6);
  auto rightOperand = std::make_shared<AST_INT>(7);
  AST_MULT astMult{token, leftOperand, rightOperand};

  REQUIRE(astMult.op().raw() == "*");
  REQUIRE(astMult.left() == leftOperand);
  REQUIRE(astMult.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astMult.left())->value() == 6);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astMult.right())->value() == 7);
}

TEST_CASE("AST_DIV class methods", "[AST_DIV]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_DIV, "/", loc};
  auto leftOperand = std::make_shared<AST_INT>(84);
  auto rightOperand = std::make_shared<AST_INT>(2);
  AST_DIV astDiv{token, leftOperand, rightOperand};

  REQUIRE(astDiv.op().raw() == "/");
  REQUIRE(astDiv.left() == leftOperand);
  REQUIRE(astDiv.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astDiv.left())->value() == 84);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astDiv.right())->value() == 2);
}

TEST_CASE("AST_MODULE class methods", "[AST_MODULE]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_MODULE, "%", loc};
  auto leftOperand = std::make_shared<AST_INT>(85);
  auto rightOperand = std::make_shared<AST_INT>(2);
  AST_MODULE astMod{token, leftOperand, rightOperand};

  REQUIRE(astMod.op().raw() == "%");
  REQUIRE(astMod.left() == leftOperand);
  REQUIRE(astMod.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astMod.left())->value() == 85);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astMod.right())->value() == 2);
}

TEST_CASE("AST_EQUAL class methods", "[AST_EQUAL]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::EQUAL, "==", loc};
  auto leftOperand = std::make_shared<AST_INT>(42);
  auto rightOperand = std::make_shared<AST_INT>(42);
  AST_EQUAL astEq{token, leftOperand, rightOperand};

  REQUIRE(astEq.op().raw() == "==");
  REQUIRE(astEq.left() == leftOperand);
  REQUIRE(astEq.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astEq.left())->value() == 42);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astEq.right())->value() == 42);
}

TEST_CASE("AST_NOT_EQUAL class methods", "[AST_NOT_EQUAL]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::NOTEQUAL, "!=", loc};
  auto leftOperand = std::make_shared<AST_INT>(42);
  auto rightOperand = std::make_shared<AST_INT>(21);
  AST_NOT_EQUAL astNeq{token, leftOperand, rightOperand};

  REQUIRE(astNeq.op().raw() == "!=");
  REQUIRE(astNeq.left() == leftOperand);
  REQUIRE(astNeq.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astNeq.left())->value() == 42);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astNeq.right())->value() == 21);
}

TEST_CASE("AST_SMALLER class methods", "[AST_SMALLER]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_SMALLER, "<", loc};
  auto leftOperand = std::make_shared<AST_INT>(21);
  auto rightOperand = std::make_shared<AST_INT>(42);
  AST_SMALLER astLt{token, leftOperand, rightOperand};

  REQUIRE(astLt.op().raw() == "<");
  REQUIRE(astLt.left() == leftOperand);
  REQUIRE(astLt.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astLt.left())->value() == 21);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astLt.right())->value() == 42);
}

TEST_CASE("AST_SMALLER_EQUAL class methods", "[AST_SMALLER_EQUAL]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::SMALLEREQUAL, "<=", loc};
  auto leftOperand = std::make_shared<AST_INT>(21);
  auto rightOperand = std::make_shared<AST_INT>(42);
  AST_SMALLER_EQUAL astLt{token, leftOperand, rightOperand};

  REQUIRE(astLt.op().raw() == "<=");
  REQUIRE(astLt.left() == leftOperand);
  REQUIRE(astLt.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astLt.left())->value() == 21);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astLt.right())->value() == 42);
}

TEST_CASE("AST_BIGGER class methods", "[AST_BIGGER]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::BIGGEREQUAL, ">", loc};
  auto leftOperand = std::make_shared<AST_INT>(21);
  auto rightOperand = std::make_shared<AST_INT>(42);
  AST_BIGGER astLt{token, leftOperand, rightOperand};

  REQUIRE(astLt.op().raw() == ">");
  REQUIRE(astLt.left() == leftOperand);
  REQUIRE(astLt.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astLt.left())->value() == 21);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astLt.right())->value() == 42);
}

TEST_CASE("AST_BIGGER_EQUAL class methods", "[AST_BIGGER_EQUAL]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::BIGGEREQUAL, ">=", loc};
  auto leftOperand = std::make_shared<AST_INT>(21);
  auto rightOperand = std::make_shared<AST_INT>(42);
  AST_BIGGER_EQUAL astLt{token, leftOperand, rightOperand};

  REQUIRE(astLt.op().raw() == ">=");
  REQUIRE(astLt.left() == leftOperand);
  REQUIRE(astLt.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astLt.left())->value() == 21);
  REQUIRE(std::dynamic_pointer_cast<AST_INT>(astLt.right())->value() == 42);
}

TEST_CASE("AST_OR class methods", "[AST_OR]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OR, "or", loc};
  auto leftOperand = std::make_shared<AST_BOOL>(true);
  auto rightOperand = std::make_shared<AST_BOOL>(false);
  AST_OR astLt{token, leftOperand, rightOperand};

  REQUIRE(astLt.op().raw() == "or");
  REQUIRE(astLt.left() == leftOperand);
  REQUIRE(astLt.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astLt.left())->value() == true);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astLt.right())->value() == false);
}

TEST_CASE("AST_AND class methods", "[AST_AND]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::AND, "and", loc};
  auto leftOperand = std::make_shared<AST_BOOL>(true);
  auto rightOperand = std::make_shared<AST_BOOL>(false);
  AST_AND astLt{token, leftOperand, rightOperand};

  REQUIRE(astLt.op().raw() == "and");
  REQUIRE(astLt.left() == leftOperand);
  REQUIRE(astLt.right() == rightOperand);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astLt.left())->value() == true);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astLt.right())->value() == false);
}