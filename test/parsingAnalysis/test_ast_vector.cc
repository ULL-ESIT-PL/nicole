#include "../../inc/parsingAnalysis/builder.h"
#include <catch2/catch_test_macros.hpp>

using namespace nicole;

TEST_CASE("AST_VECTOR class methods", "[AST_VECTOR]") {
  Location loc{"file", 0, 0};
  Token token{TokenType::OPERATOR_ADD, "+", loc};
  auto astBool1 = *Builder::createBool(SourceLocation{token, token}, true);
  auto astBool2 = *Builder::createBool(SourceLocation{token, token}, false);

  std::vector<std::shared_ptr<AST>> values{astBool1, astBool2};
  AST_VECTOR astVector{0, SourceLocation{token, token}, values};

  REQUIRE(astVector.values().size() == 2);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astVector.values()[0])->value() ==
          true);
  REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astVector.values()[1])->value() ==
          false);
}
