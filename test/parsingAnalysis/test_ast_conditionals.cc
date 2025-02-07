#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_CASE class methods", "[AST_CASE]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto statement =
      std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(true));
  auto body = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_CASE astCase{condition, body};

  REQUIRE(astCase.match() == condition);
  REQUIRE(astCase.body() == body);
}

TEST_CASE("AST_DEFAULT class methods", "[AST_DEFAULT]") {
  auto statement =
      std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(false));
  auto body = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_DEFAULT astDefault{body};

  REQUIRE(astDefault.body() == body);
}

TEST_CASE("AST_ELSE_IF class methods", "[AST_ELSE_IF]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto statement =
      std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(true));
  auto body = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_ELSE_IF astElseIf{condition, body};

  REQUIRE(astElseIf.condition() == condition);
  REQUIRE(astElseIf.body() == body);
}

TEST_CASE("AST_IF class methods", "[AST_IF]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto statement =
      std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(true));
  auto body = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});

  auto elseCondition =
      std::make_shared<AST_CONDITION>(std::make_shared<AST_BOOL>(false));
  auto elseStatement = std::make_shared<AST_STATEMENT>(elseCondition);
  auto elseIfBody = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{elseStatement});
  auto elseIf = std::make_shared<AST_ELSE_IF>(elseCondition, elseIfBody);

  auto elseBody = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});

  AST_IF astIf{condition, body, {elseIf}, elseBody};

  REQUIRE(astIf.condition() == condition);
  REQUIRE(astIf.body() == body);
  REQUIRE(astIf.elseIf().size() == 1);
  REQUIRE(astIf.elseIf()[0] == elseIf);
  REQUIRE(astIf.elseBody() == elseBody);
}

TEST_CASE("AST_SWITCH class methods", "[AST_SWITCH]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto statement =
      std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(true));
  auto body = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  auto case1 = std::make_shared<AST_CASE>(condition, body);

  auto defaultBody = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  auto defaultCase = std::make_shared<AST_DEFAULT>(defaultBody);

  AST_SWITCH astSwitch{condition, {case1}, defaultCase};

  REQUIRE(astSwitch.condition() == condition);
  REQUIRE(astSwitch.cases().size() == 1);
  REQUIRE(astSwitch.cases()[0] == case1);
  REQUIRE(astSwitch.defaultCase() == defaultCase);
}

TEST_CASE("AST_TERNARY class methods", "[AST_TERNARY]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto first = std::make_shared<AST_BOOL>(false);
  auto second = std::make_shared<AST_BOOL>(true);

  AST_TERNARY astTernary{condition, first, second};

  REQUIRE(astTernary.condition() == condition);
  REQUIRE(astTernary.first() == first);
  REQUIRE(astTernary.second() == second);
}
