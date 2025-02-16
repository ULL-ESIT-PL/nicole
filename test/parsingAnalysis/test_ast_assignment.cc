#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace nicole;

TEST_CASE("AST_ASSIGNMENT class methods", "[AST_ASSIGNMENT]") {
  const Token op{TokenType::SELF_ADD, "+=", Location{"archivo.cpp", 10, 5}};
  auto astBool = *Builder::createBool(true);
  auto left = *Builder::createBool(true);
  AST_ASSIGNMENT astSelfAdd{op, left, astBool};

  REQUIRE(astSelfAdd.op().raw() == "+=");
  REQUIRE(astSelfAdd.left() == left);
  REQUIRE(astSelfAdd.value() == astBool);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astSelfAdd.value())->value() ==
          true);
}