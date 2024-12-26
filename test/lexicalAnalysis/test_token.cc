#include "../inc/lexicalAnalysis/token.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Pruebas de la clase Token", "[token]") {
  using nicole::Location;
  using nicole::Token;
  using nicole::TokenType;
  using std::filesystem::path;

  SECTION("Constructor y métodos de acceso") {
    const Token token{TokenType::NUMBER_INT, "123", Location{"archivo.cpp", 10, 5}};
    REQUIRE(token.type() == TokenType::NUMBER_INT);
    REQUIRE(token.raw() == "123");
    REQUIRE(token.location().file() == path{"archivo.cpp"});
    REQUIRE(token.location().row() == 10);
    REQUIRE(token.location().col() == 5);
  }

  SECTION("Información de ubicación formateada") {
    const Token token{TokenType::STRING, "\"hola\"", Location{"main.cpp", 20, 15}};
    REQUIRE(token.locInfo() == "main.cpp 20:15");
  }

  SECTION("Tokens con diferentes tipos y valores") {
    const Token tokenFloat{TokenType::NUMBER_FLOAT, "3.14",
                     Location{"math.cpp", 5, 10}};
    REQUIRE(tokenFloat.type() == TokenType::NUMBER_FLOAT);
    REQUIRE(tokenFloat.raw() == "3.14");

    const Token tokenId{TokenType::ID, "variable", Location{"vars.cpp", 8, 2}};
    REQUIRE(tokenId.type() == TokenType::ID);
    REQUIRE(tokenId.raw() == "variable");
  }

  SECTION("Comparación de tokens") {
    const Token token1{TokenType::LET, "let", Location{"script.nic", 1, 1}};
    const Token token2{TokenType::LET, "let", Location{"script.nic", 1, 1}};
    const Token token3{TokenType::CONST, "const", Location{"script.nic", 1, 5}};

    REQUIRE(token1.type() == token2.type());
    REQUIRE(token1.raw() == token2.raw());
    REQUIRE(token1.location().file() == token2.location().file());
    REQUIRE(token1.location().row() == token2.location().row());
    REQUIRE(token1.location().col() == token2.location().col());

    REQUIRE(token1.type() != token3.type());
    REQUIRE(token1.raw() != token3.raw());
    REQUIRE(token1.location().col() != token3.location().col());
  }
}
