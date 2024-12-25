#include <catch2/catch_test_macros.hpp>

#include "../inc/lexicalAnalysis/location.h"

TEST_CASE( "Factorials are computed", "[factorial2]" ) {
    REQUIRE( nicole::Location{"", 10, 10}.row() == 10);
    REQUIRE( nicole::Location{"", 10, 10}.row() == 10);
    REQUIRE( nicole::Location{"", 10, 10}.row() == 10); 
} 