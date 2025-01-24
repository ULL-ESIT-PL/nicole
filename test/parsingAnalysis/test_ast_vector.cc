#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../inc/parsingAnalysis/ast/vector/ast_vector.h"
#include <catch2/catch_test_macros.hpp>

using namespace nicole;

TEST_CASE("AST_VECTOR class methods", "[AST_VECTOR]") {
    auto astBool1 = std::make_shared<AST_BOOL>(true);
    auto astBool2 = std::make_shared<AST_BOOL>(false);

    std::vector<std::shared_ptr<AST>> values{astBool1, astBool2};
    AST_VECTOR astVector{values};

    REQUIRE(astVector.values().size() == 2);
    REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astVector.values()[0])->value() == true);
    REQUIRE(std::dynamic_pointer_cast<AST_BOOL>(astVector.values()[1])->value() == false);
}

