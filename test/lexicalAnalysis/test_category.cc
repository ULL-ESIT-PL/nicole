#include "../inc/lexicalAnalysis/category.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Pruebas de la clase Category", "[category]") {
  using nicole::Category;
  using nicole::Location;
  using nicole::Token;
  using nicole::TokenType;

  SECTION("Constructor y métodos de acceso") {
    Category category{TokenType::NUMBER_INT, "\\d+", false};
    REQUIRE(category.type() == TokenType::NUMBER_INT);
    REQUIRE(category.rawPattern() == "\\d+");
    REQUIRE(category.skip() == false);
  }

  SECTION("Coincidencia de patrones con tokens") {
    Category category{TokenType::ID, "[a-zA-Z_][a-zA-Z0-9_]*", false};
    Token token{TokenType::ID, "variable123", Location{"archivo.cpp", 1, 1}};
    REQUIRE(category.matchToken(token));
  }

  SECTION("No coincidencia de patrones con tokens") {
    Category category{TokenType::NUMBER_FLOAT, "\\d+\\.\\d+", false};
    Token token{TokenType::NUMBER_FLOAT, "123", Location{"archivo.cpp", 1, 1}};
    REQUIRE_FALSE(category.matchToken(token));
  }

  SECTION("Tokens que deben ser omitidos") {
    Category category{TokenType::COMMENT, "//.*", true};
    REQUIRE(category.skip() == true);
  }
}
