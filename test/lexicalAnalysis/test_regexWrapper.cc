#include "../inc/lexicalAnalysis/regexWrapper.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Pruebas de la clase RegexWrapper", "[regexwrapper]") {
  using nicole::RegexWrapper;

  SECTION("Constructor y métodos de acceso") {
    RegexWrapper regex{"\\d+"};
    REQUIRE(regex.str() == "\\d+");
    REQUIRE(regex.match("12345"));
    REQUIRE_FALSE(regex.match("abc"));
  }

  SECTION("Coincidencia exacta") {
    RegexWrapper regex{"^abc$"};
    REQUIRE(regex.match("abc"));
    REQUIRE_FALSE(regex.match(" abc "));
    REQUIRE_FALSE(regex.match("abcd"));
  }

  SECTION("Patrón vacío") {
    RegexWrapper regex{""};
    REQUIRE(regex.str().empty());
    REQUIRE(regex.match(""));
    REQUIRE_FALSE(regex.match("a"));
  }

  SECTION("Patrón de letras minúsculas") {
    RegexWrapper regex{"^[a-z]+$"};
    REQUIRE(regex.match("abcxyz"));
    REQUIRE_FALSE(regex.match("ABC"));
    REQUIRE_FALSE(regex.match("abc123"));
  }

  SECTION("Patrón de dígitos y letras") {
    RegexWrapper regex{"^[a-zA-Z0-9]+$"};
    REQUIRE(regex.match("abc123XYZ"));
    REQUIRE_FALSE(regex.match("abc-123"));
    REQUIRE_FALSE(regex.match("abc 123"));
  }
}