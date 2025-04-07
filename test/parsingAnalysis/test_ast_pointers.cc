#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>

using namespace nicole;

TEST_CASE("AST_DELETE class methods", "[AST_DELETE]") {
  auto value = *Builder::createBool(true);
  AST_DELETE astDelete{0,value};

  REQUIRE(astDelete.value() == value);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astDelete.value())->value() ==
          true);
}

TEST_CASE("AST_DEREF class methods", "[AST_DEREF]") {
  auto value = *Builder::createBool(false);
  AST_DEREF astDeref{0,value};

  REQUIRE(astDeref.value() == value);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astDeref.value())->value() ==
          false);
}

TEST_CASE("AST_NEW class methods", "[AST_NEW]") {
  auto value = *Builder::createBool(true);
  AST_NEW astNew{0,value};

  REQUIRE(astNew.value() == value);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astNew.value())->value() == true);
}