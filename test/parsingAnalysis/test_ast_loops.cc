#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_DO_WHILE class methods", "[AST_DO_WHILE]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto statement = *Builder::createStatement(condition);
  auto body = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_DO_WHILE astDoWhile{0,body, condition};

  REQUIRE(astDoWhile.condition() == condition);
  REQUIRE(astDoWhile.body() == body);
}

TEST_CASE("AST_FOR class methods", "[AST_FOR]") {
  auto init = std::vector<std::shared_ptr<AST>>{
      *Builder::createStatement(*Builder::createBool(true))};
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto update = std::vector<std::shared_ptr<AST>>{
      *Builder::createStatement(*Builder::createBool(false))};
  auto statement = *Builder::createStatement(condition);
  auto body = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});

  AST_FOR astFor{0,init, condition, update, body};

  REQUIRE(astFor.init()[0] == init[0]);
  REQUIRE(astFor.condition() == condition);
  REQUIRE(astFor.update()[0] == update[0]);
  REQUIRE(astFor.body() == body);
}

TEST_CASE("AST_WHILE class methods", "[AST_WHILE]") {
  auto condition =*Builder::createCondition(*Builder::createBool(true));
  auto statement = *Builder::createStatement(condition);
  auto body = *Builder::createBody(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_WHILE astWhile{0,condition, body};

  REQUIRE(astWhile.condition() == condition);
  REQUIRE(astWhile.body() == body);
}

TEST_CASE("AST_STOP within loops", "[AST_STOP]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto body =
      *Builder::createBody(std::vector<std::shared_ptr<AST_STATEMENT>>{});

  SECTION("AST_STOP in AST_FOR") {
    auto loop = *Builder::createFor(
        std::vector<std::shared_ptr<AST>>{}, condition,
        std::vector<std::shared_ptr<AST>>{}, body);
    AST_STOP astStop{0,loop};
    REQUIRE(astStop.fatherLoop() == loop);
  }

  SECTION("AST_STOP in AST_WHILE") {
    auto loop = *Builder::createWhile(condition, body);
    AST_STOP astStop{0,loop};
    REQUIRE(astStop.fatherLoop() == loop);
  }

  SECTION("AST_STOP in AST_DO_WHILE") {
    auto loop = *Builder::createDoWhile(body, condition);
    AST_STOP astStop{0,loop};
    REQUIRE(astStop.fatherLoop() == loop);
  }
}

TEST_CASE("AST_PASS within loops", "[AST_PASS]") {
  auto condition = *Builder::createCondition(*Builder::createBool(true));
  auto body =
      *Builder::createBody(std::vector<std::shared_ptr<AST_STATEMENT>>{});

  SECTION("AST_PASS in AST_FOR") {
    auto loop = *Builder::createFor(
        std::vector<std::shared_ptr<AST>>{}, condition,
        std::vector<std::shared_ptr<AST>>{}, body);
    AST_PASS astPass{0,loop};
    REQUIRE(astPass.fatherLoop() == loop);
  }

  SECTION("AST_PASS in AST_WHILE") {
    auto loop = *Builder::createWhile(condition, body);
    AST_PASS astPass{0,loop};
    REQUIRE(astPass.fatherLoop() == loop);
  }

  SECTION("AST_PASS in AST_DO_WHILE") {
    auto loop = *Builder::createDoWhile(body, condition);
    AST_PASS astPass{0,loop};
    REQUIRE(astPass.fatherLoop() == loop);
  }
}