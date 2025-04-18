#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST class methods", "[AST]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto ast = *Builder::createBool(SourceLocation{token, token}, true);

  SECTION("Constructor and accessors") {
    REQUIRE(ast->type() == AST_TYPE::BOOL);
  }

  SECTION("Father management") {
    ast->setFather(ast);
    REQUIRE(ast->father() == ast);
  }
}

TEST_CASE("AST_BOOL class methods", "[AST_BOOL]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  AST_BOOL astBool{0, SourceLocation{token, token}, true};

  REQUIRE(astBool.type() == AST_TYPE::BOOL);
  REQUIRE(astBool.value() == true);
}

TEST_CASE("AST_CHAR class methods", "[AST_CHAR]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  AST_CHAR astChar{0, SourceLocation{token, token}, 'a'};

  REQUIRE(astChar.type() == AST_TYPE::CHAR);
  REQUIRE(astChar.value() == 'a');
}

TEST_CASE("AST_DOUBLE class methods", "[AST_DOUBLE]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  AST_DOUBLE astDouble{0, SourceLocation{token, token}, 3.14};

  REQUIRE(astDouble.type() == AST_TYPE::DOUBLE);
  REQUIRE(astDouble.value() == 3.14);
}

TEST_CASE("AST_FLOAT class methods", "[AST_FLOAT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  AST_FLOAT astFloat{0, SourceLocation{token, token}, 2.71f};

  REQUIRE(astFloat.type() == AST_TYPE::FLOAT);
  REQUIRE(astFloat.value() == 2.71f);
}

TEST_CASE("AST_INT class methods", "[AST_INT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  AST_INT astInt{0, SourceLocation{token, token}, 42};

  REQUIRE(astInt.type() == AST_TYPE::INT);
  REQUIRE(astInt.value() == 42);
}

TEST_CASE("AST_NULL class methods", "[AST_NULL]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  AST_NULL astNull{0, SourceLocation{token, token}};

  REQUIRE(astNull.type() == AST_TYPE::NULLPTR);
}

TEST_CASE("AST_STRING class methods", "[AST_STRING]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  AST_STRING astString{0, SourceLocation{token, token}, "hello"};

  REQUIRE(astString.type() == AST_TYPE::STRING);
  REQUIRE(astString.value() == "hello");
}
