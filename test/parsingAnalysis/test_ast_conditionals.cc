#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_CASE class methods", "[AST_CASE]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto condition = *Builder::createCondition(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto statement = *Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto body = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_CASE astCase{0, SourceLocation{token, token}, condition, body};

  REQUIRE(astCase.match() == condition);
  REQUIRE(astCase.body() == body);
}

TEST_CASE("AST_DEFAULT class methods", "[AST_DEFAULT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto statement = *Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, false));
  auto body = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_DEFAULT astDefault{0, SourceLocation{token, token}, body};

  REQUIRE(astDefault.body() == body);
}

TEST_CASE("AST_ELSE_IF class methods", "[AST_ELSE_IF]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto condition = *Builder::createCondition(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto statement = *Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto body = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_ELSE_IF astElseIf{0, SourceLocation{token, token}, condition, body};

  REQUIRE(astElseIf.condition() == condition);
  REQUIRE(astElseIf.body() == body);
}

TEST_CASE("AST_IF class methods", "[AST_IF]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto condition = *Builder::createCondition(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto statement = *Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto body = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});

  auto elseCondition = *Builder::createCondition(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, false));
  auto elseStatement =
      *Builder::createStatement(SourceLocation{token, token}, elseCondition);
  auto elseIfBody = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{elseStatement});
  auto elseIf = *Builder::createElseIf(SourceLocation{token, token},
                                       elseCondition, elseIfBody);

  auto elseBody = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});

  AST_IF astIf{0,       SourceLocation{token, token}, condition, body, {elseIf},
               elseBody};

  REQUIRE(astIf.condition() == condition);
  REQUIRE(astIf.body() == body);
  REQUIRE(astIf.elseIf().size() == 1);
  REQUIRE(astIf.elseIf()[0] == elseIf);
  REQUIRE(astIf.elseBody() == elseBody);
}

TEST_CASE("AST_SWITCH class methods", "[AST_SWITCH]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto condition = *Builder::createCondition(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto statement = *Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto body = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  auto case1 = std::make_shared<AST_CASE>(0, SourceLocation{token, token},
                                          condition, body);

  auto defaultBody = *Builder::createBody(
      SourceLocation{token, token},
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  auto defaultCase =
      *Builder::createDefault(SourceLocation{token, token}, defaultBody);

  AST_SWITCH astSwitch{
      0, SourceLocation{token, token}, condition, {case1}, defaultCase};

  REQUIRE(astSwitch.condition() == condition);
  REQUIRE(astSwitch.cases().size() == 1);
  REQUIRE(astSwitch.cases()[0] == case1);
  REQUIRE(astSwitch.defaultCase() == defaultCase);
}

TEST_CASE("AST_TERNARY class methods", "[AST_TERNARY]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto condition = *Builder::createCondition(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto first = *Builder::createBool(SourceLocation{token, token}, false);
  auto second = *Builder::createBool(SourceLocation{token, token}, true);

  AST_TERNARY astTernary{0, SourceLocation{token, token}, condition, first,
                         second};

  REQUIRE(astTernary.condition() == condition);
  REQUIRE(astTernary.first() == first);
  REQUIRE(astTernary.second() == second);
}
