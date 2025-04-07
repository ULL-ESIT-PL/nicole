#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST class methods", "[AST]") {
  auto ast = *Builder::createBool(true);

  SECTION("Constructor and accessors") {
    REQUIRE(ast->type() == AST_TYPE::BOOL);
  }

  SECTION("Father management") {
    ast->setFather(ast);
    REQUIRE(ast->father() == ast);
  }
}

TEST_CASE("AST_BOOL class methods", "[AST_BOOL]") {
  AST_BOOL astBool{0,true};

  REQUIRE(astBool.type() == AST_TYPE::BOOL);
  REQUIRE(astBool.value() == true);
}

TEST_CASE("AST_CHAR class methods", "[AST_CHAR]") {
  AST_CHAR astChar{0,'a'};

  REQUIRE(astChar.type() == AST_TYPE::CHAR);
  REQUIRE(astChar.value() == 'a');
}

TEST_CASE("AST_DOUBLE class methods", "[AST_DOUBLE]") {
  AST_DOUBLE astDouble{0,3.14};

  REQUIRE(astDouble.type() == AST_TYPE::DOUBLE);
  REQUIRE(astDouble.value() == 3.14);
}

TEST_CASE("AST_FLOAT class methods", "[AST_FLOAT]") {
  AST_FLOAT astFloat{0,2.71f};

  REQUIRE(astFloat.type() == AST_TYPE::FLOAT);
  REQUIRE(astFloat.value() == 2.71f);
}

TEST_CASE("AST_INT class methods", "[AST_INT]") {
  AST_INT astInt{0,42};

  REQUIRE(astInt.type() == AST_TYPE::INT);
  REQUIRE(astInt.value() == 42);
}

TEST_CASE("AST_NULL class methods", "[AST_NULL]") {
  AST_NULL astNull{0,};

  REQUIRE(astNull.type() == AST_TYPE::NULLPTR);
}

TEST_CASE("AST_STRING class methods", "[AST_STRING]") {
  AST_STRING astString{0,"hello"};

  REQUIRE(astString.type() == AST_TYPE::STRING);
  REQUIRE(astString.value() == "hello");
}
