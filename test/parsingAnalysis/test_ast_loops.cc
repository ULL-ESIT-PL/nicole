#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_DO_WHILE class methods", "[AST_DO_WHILE]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto condition = *Builder::createCondition(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto statement =
      *Builder::createStatement(SourceLocation{token, token}, condition);
  auto body = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_DO_WHILE astDoWhile{0, SourceLocation{token, token}, body, condition};

  REQUIRE(astDoWhile.condition() == condition);
  REQUIRE(astDoWhile.body() == body);
}

TEST_CASE("AST_FOR class methods", "[AST_FOR]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto init = std::vector<std::shared_ptr<AST>>{*Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true))};
  auto condition = *Builder::createCondition(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto update = std::vector<std::shared_ptr<AST>>{*Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, false))};
  auto statement =
      *Builder::createStatement(SourceLocation{token, token}, condition);
  auto body = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});

  AST_FOR astFor{0,   SourceLocation{token, token}, init, condition, update,
                 body};

  REQUIRE(astFor.init()[0] == init[0]);
  REQUIRE(astFor.condition() == condition);
  REQUIRE(astFor.update()[0] == update[0]);
  REQUIRE(astFor.body() == body);
}

TEST_CASE("AST_WHILE class methods", "[AST_WHILE]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto condition = *Builder::createCondition(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto statement =
      *Builder::createStatement(SourceLocation{token, token}, condition);
  auto body = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_WHILE astWhile{0, SourceLocation{token, token}, condition, body};

  REQUIRE(astWhile.condition() == condition);
  REQUIRE(astWhile.body() == body);
}