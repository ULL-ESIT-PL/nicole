#include <catch2/catch_test_macros.hpp>
#include "../inc/lexicalAnalysis/location.h"

TEST_CASE("Pruebas de la clase Location", "[location]") {
    using nicole::Location;
    using std::filesystem::path;

    SECTION("Constructor y métodos de acceso") {
        const Location loc{"archivo.cpp", 5, 15};
        REQUIRE(loc.file() == path{"archivo.cpp"});
        REQUIRE(loc.row() == 5);
        REQUIRE(loc.col() == 15);
    }

    SECTION("Valores límite para fila y columna") {
        const Location loc{"archivo.cpp", 0, 0};
        REQUIRE(loc.row() == 0);
        REQUIRE(loc.col() == 0);

        const Location locMax{"archivo.cpp", std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max()};
        REQUIRE(locMax.row() == std::numeric_limits<std::size_t>::max());
        REQUIRE(locMax.col() == std::numeric_limits<std::size_t>::max());
    }

    SECTION("Comparación de ubicaciones") {
        const Location loc1{"archivo.cpp", 5, 15};
        const Location loc2{"archivo.cpp", 5, 15};
        const Location loc3{"archivo.cpp", 6, 10};

        REQUIRE(loc1.file() == loc2.file());
        REQUIRE(loc1.row() == loc2.row());
        REQUIRE(loc1.col() == loc2.col());

        REQUIRE(loc1.file() == loc3.file());
        REQUIRE(loc1.row() != loc3.row());
        REQUIRE(loc1.col() != loc3.col());
    }

    SECTION("Ubicaciones en diferentes archivos") {
        const Location loc1{"archivo1.cpp", 5, 15};
        const Location loc2{"archivo2.cpp", 5, 15};

        REQUIRE(loc1.file() != loc2.file());
        REQUIRE(loc1.row() == loc2.row());
        REQUIRE(loc1.col() == loc2.col());
    }
}
