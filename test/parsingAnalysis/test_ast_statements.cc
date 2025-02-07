#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

using namespace nicole;

TEST_CASE("AST_BODY class methods", "[AST_BODY]") {
  auto astBool1 =
      std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(true));
  auto astBool2 =
      std::make_shared<AST_STATEMENT>(std::make_shared<AST_BOOL>(false));
  std::vector<std::shared_ptr<AST_STATEMENT>> body{astBool1, astBool2};

  AST_BODY astBody{body};

  REQUIRE(astBody.body().size() == 2);
  REQUIRE(astBody.body()[0] == astBool1);
  REQUIRE(astBody.body()[1] == astBool2);
}

TEST_CASE("AST_STATEMENT class methods", "[AST_STATEMENT]") {
  auto astBool = std::make_shared<AST_BOOL>(true);

  AST_STATEMENT astStatement{astBool};

  REQUIRE(astStatement.expression() == astBool);
  REQUIRE(
      std::dynamic_pointer_cast<AST_BOOL>(astStatement.expression())->value() ==
      true);
}
