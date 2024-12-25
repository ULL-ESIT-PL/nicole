#include <catch2/catch_test_macros.hpp>

#include "../inc/lexicalAnalysis/location.h"

uint32_t factorial( uint32_t number ) {
    return number <= 1 ? number : factorial(number-1) * number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( factorial( 1) == 1 );
    REQUIRE( factorial( 2) == 2 );
    REQUIRE( factorial( 3) == 6 );
    REQUIRE( factorial(10) == 3'628'800 );
    REQUIRE( nicole::Location{"", 10, 10}.col() == 10);
    REQUIRE( nicole::Location{"", 10, 10}.col() == 10);
    REQUIRE( nicole::Location{"", 10, 10}.col() == 10); 
} 