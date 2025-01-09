#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_doWhile.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_for.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_pass.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_stop.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_while.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_DO_WHILE class methods", "[AST_DO_WHILE]") {
  auto condition = std::make_shared<AST_BOOL>(true);
  auto statement = std::make_shared<AST_STATEMENT>(condition);
  auto body = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_DO_WHILE astDoWhile{body, condition};

  REQUIRE(astDoWhile.condition() == condition);
  REQUIRE(astDoWhile.body() == body);
}

TEST_CASE("AST_FOR class methods", "[AST_FOR]") {
  auto init =
      std::make_shared<AST_COMMA>(std::vector<std::shared_ptr<AST_STATEMENT>>{
          std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(true))});
  auto condition = std::make_shared<AST_BOOL>(false);
  auto update =
      std::make_shared<AST_COMMA>(std::vector<std::shared_ptr<AST_STATEMENT>>{
          std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(false))});
  auto statement = std::make_shared<AST_STATEMENT>(condition);
  auto body = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_FOR astFor{init, condition, update, body};

  REQUIRE(astFor.init() == init);
  REQUIRE(astFor.condition() == condition);
  REQUIRE(astFor.update() == update);
  REQUIRE(astFor.body() == body);
}

TEST_CASE("AST_WHILE class methods", "[AST_WHILE]") {
  auto condition = std::make_shared<AST_BOOL>(true);
  auto statement = std::make_shared<AST_STATEMENT>(condition);
  auto body = std::make_shared<AST_BODY>(
      std::vector<std::shared_ptr<AST_STATEMENT>>{statement});
  AST_WHILE astWhile{condition, body};

  REQUIRE(astWhile.condition() == condition);
  REQUIRE(astWhile.body() == body);
}

TEST_CASE("AST_STOP within loops", "[AST_STOP]") {
  auto condition = std::make_shared<AST_BOOL>(true);
  auto body =
      std::make_shared<AST_BODY>(std::vector<std::shared_ptr<AST_STATEMENT>>{});

  SECTION("AST_STOP in AST_FOR") {
    auto loop = std::make_shared<AST_FOR>(nullptr, condition, nullptr, body);
    AST_STOP astStop{loop};
    REQUIRE(astStop.fatherLoop() == loop);
  }

  SECTION("AST_STOP in AST_WHILE") {
    auto loop = std::make_shared<AST_WHILE>(condition, body);
    AST_STOP astStop{loop};
    REQUIRE(astStop.fatherLoop() == loop);
  }

  SECTION("AST_STOP in AST_DO_WHILE") {
    auto loop = std::make_shared<AST_DO_WHILE>(body, condition);
    AST_STOP astStop{loop};
    REQUIRE(astStop.fatherLoop() == loop);
  }
}

TEST_CASE("AST_PASS within loops", "[AST_PASS]") {
  auto condition = std::make_shared<AST_BOOL>(true);
  auto body =
      std::make_shared<AST_BODY>(std::vector<std::shared_ptr<AST_STATEMENT>>{});

  SECTION("AST_PASS in AST_FOR") {
    auto loop = std::make_shared<AST_FOR>(nullptr, condition, nullptr, body);
    AST_PASS astPass{loop};
    REQUIRE(astPass.fatherLoop() == loop);
  }

  SECTION("AST_PASS in AST_WHILE") {
    auto loop = std::make_shared<AST_WHILE>(condition, body);
    AST_PASS astPass{loop};
    REQUIRE(astPass.fatherLoop() == loop);
  }

  SECTION("AST_PASS in AST_DO_WHILE") {
    auto loop = std::make_shared<AST_DO_WHILE>(body, condition);
    AST_PASS astPass{loop};
    REQUIRE(astPass.fatherLoop() == loop);
  }
}