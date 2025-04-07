#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_CASE class methods", "[AST_CASE]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto statement = *Builder::createStatement(*Builder::createBool(true));
  auto body = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_CASE astCase{0,condition, body};

  REQUIRE(astCase.match() == condition);
  REQUIRE(astCase.body() == body);
}

TEST_CASE("AST_DEFAULT class methods", "[AST_DEFAULT]") {
  auto statement = *Builder::createStatement(*Builder::createBool(false));
  auto body = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_DEFAULT astDefault{0,body};

  REQUIRE(astDefault.body() == body);
}

TEST_CASE("AST_ELSE_IF class methods", "[AST_ELSE_IF]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto statement = *Builder::createStatement(*Builder::createBool(true));
  auto body = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_ELSE_IF astElseIf{0,condition, body};

  REQUIRE(astElseIf.condition() == condition);
  REQUIRE(astElseIf.body() == body);
}

TEST_CASE("AST_IF class methods", "[AST_IF]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto statement = *Builder::createStatement(*Builder::createBool(true));
  auto body = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});

  auto elseCondition = *Builder::createCondition(*Builder::createBool(false));
  auto elseStatement = *Builder::createStatement(elseCondition);
  auto elseIfBody = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{elseStatement});
  auto elseIf = *Builder::createElseIf(elseCondition, elseIfBody);

  auto elseBody = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});

  AST_IF astIf{0,condition, body, {elseIf}, elseBody};

  REQUIRE(astIf.condition() == condition);
  REQUIRE(astIf.body() == body);
  REQUIRE(astIf.elseIf().size() == 1);
  REQUIRE(astIf.elseIf()[0] == elseIf);
  REQUIRE(astIf.elseBody() == elseBody);
}

TEST_CASE("AST_SWITCH class methods", "[AST_SWITCH]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto statement = *Builder::createStatement(*Builder::createBool(true));
  auto body = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  auto case1 = std::make_shared<AST_CASE>(0,condition, body);

  auto defaultBody = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  auto defaultCase = *Builder::createDefault(defaultBody);

  AST_SWITCH astSwitch{0,condition, {case1}, defaultCase};

  REQUIRE(astSwitch.condition() == condition);
  REQUIRE(astSwitch.cases().size() == 1);
  REQUIRE(astSwitch.cases()[0] == case1);
  REQUIRE(astSwitch.defaultCase() == defaultCase);
}

TEST_CASE("AST_TERNARY class methods", "[AST_TERNARY]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto first = *Builder::createBool(false);
  auto second = *Builder::createBool(true);

  AST_TERNARY astTernary{0,condition, first, second};

  REQUIRE(astTernary.condition() == condition);
  REQUIRE(astTernary.first() == first);
  REQUIRE(astTernary.second() == second);
}
