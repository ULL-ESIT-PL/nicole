#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

using namespace nicole;

TEST_CASE("AST_BODY class methods", "[AST_BODY]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto astBool1 = *Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, true));
  auto astBool2 = *Builder::createStatement(
      SourceLocation{token, token},
      *Builder::createBool(SourceLocation{token, token}, false));
  std::vector<std::shared_ptr<AST_STATEMENT>> body{astBool1, astBool2};

  AST_BODY astBody{0, SourceLocation{token, token}, body};

  REQUIRE(astBody.body().size() == 2);
  REQUIRE(astBody.body()[0] == astBool1);
  REQUIRE(astBody.body()[1] == astBool2);
}

TEST_CASE("AST_STATEMENT class methods", "[AST_STATEMENT]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto astBool = *Builder::createBool(SourceLocation{token, token}, true);

  AST_STATEMENT astStatement{0, SourceLocation{token, token}, astBool};

  REQUIRE(astStatement.expression() == astBool);
  REQUIRE(
      std::dynamic_pointer_cast<AST_BOOL>(astStatement.expression())->value() ==
      true);
}
