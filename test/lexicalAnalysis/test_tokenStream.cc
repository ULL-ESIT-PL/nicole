#include "../../inc/lexicalAnalysis/token.h"
#include "../../inc/lexicalAnalysis/tokenStream.h"
#include "../../inc/lexicalAnalysis/type.h"
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

TEST_CASE("TokenStream operations", "[TokenStream]") {
  using namespace nicole;

  // Values of Location are not relevant here

  std::vector<Token> tokens = {
      Token(TokenType::ID, "var", Location{"", 0, 0}),
      Token(TokenType::ASSIGNMENT, "=", Location{"", 0, 0}),
      Token(TokenType::NUMBER_INT, "42", Location{"", 0, 0}),
      Token(TokenType::SEMICOLON, ";", Location{"", 0, 0})};

  TokenStream tokenStream(tokens);

  SECTION("Initial state") {
    REQUIRE(tokenStream.size() == tokens.size());
    REQUIRE(tokenStream.currentPos() == 0);
    REQUIRE_FALSE(tokenStream.isEnd());
  }

  SECTION("Eating tokens") {
    REQUIRE(tokenStream.eat().has_value());
    REQUIRE(tokenStream.currentPos() == 1);
    REQUIRE(tokenStream.eat().has_value());
    REQUIRE(tokenStream.currentPos() == 2);
  }

  SECTION("Current token") {
    auto currentToken = tokenStream.current();
    REQUIRE(currentToken.has_value());
    REQUIRE(currentToken->type() == TokenType::ID);
    REQUIRE(currentToken->raw() == "var");
  }

  SECTION("Look ahead") {
    auto lookaheadToken = tokenStream.lookAhead(2);
    REQUIRE(lookaheadToken.has_value());
    REQUIRE(lookaheadToken->type() == TokenType::NUMBER_INT);
    REQUIRE(lookaheadToken->raw() == "42");
  }

  SECTION("Is current token type") {
    auto isCurrentId = tokenStream.isCurrentTokenType(TokenType::ID);
    REQUIRE(isCurrentId.has_value());
    REQUIRE(isCurrentId.value() == true);
  }

  SECTION("Is token ahead before semicolon") {
    REQUIRE(tokenStream.isTokenAheadBeforeSemicolon(TokenType::NUMBER_INT));
    REQUIRE_FALSE(tokenStream.isTokenAheadBeforeSemicolon(TokenType::STRING));
  }

  SECTION("Insert after") {
    std::vector<Token> newTokens = {
        Token(TokenType::PRINT, "print", Location{"", 0, 0}),
        Token(TokenType::STRING, "\"Hello World\"", Location{"", 0, 0})};
    TokenStream newTokenStream(newTokens);
    REQUIRE(tokenStream.insertAfter(newTokenStream, 2).has_value());
    REQUIRE(tokenStream.size() == tokens.size() + newTokens.size());
  }
}

TEST_CASE("TokenStream: Manejo de errores", "[TokenStream]") {
  using namespace nicole;
  // Tokens de ejemplo para las pruebas
  std::vector<Token> tokens = {
      Token(TokenType::NUMBER_INT, "1", Location{"", 0, 0}),
      Token(TokenType::OPERATOR_ADD, "+", Location{"", 0, 0}),
      Token(TokenType::NUMBER_INT, "2", Location{"", 0, 0}),
      Token(TokenType::SEMICOLON, ";", Location{"", 0, 0})};

  TokenStream tokenStream(tokens);

  SECTION("Error en 'eat' al intentar avanzar más allá del final") {
    // Avanzar hasta el final del stream
    while (!tokenStream.isEnd()) {
      auto result = tokenStream.eat();
      REQUIRE(result.has_value());
    }
    // Intentar avanzar más allá del final
    auto result = tokenStream.eat();
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error().type() == ERROR_TYPE::EAT);
  }

  SECTION("Error en 'current' al acceder fuera de los límites") {
    // Avanzar hasta el final del stream
    while (!tokenStream.isEnd()) {
      auto result = tokenStream.eat();
      REQUIRE(result.has_value());
    }
    // Intentar obtener el token actual más allá del final
    auto result = tokenStream.current();
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error().type() == ERROR_TYPE::CURRENT);
  }

  SECTION("Error en 'lookAhead' con posición inválida") {
    // Intentar mirar más allá del final del stream
    auto result = tokenStream.lookAhead(tokens.size());
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error().type() == ERROR_TYPE::LOOK_AHEAD);
  }

  SECTION(
      "Error en 'isCurrentTokenType' al verificar tipo fuera de los límites") {
    // Avanzar hasta el final del stream
    while (!tokenStream.isEnd()) {
      auto result = tokenStream.eat();
      REQUIRE(result.has_value());
    }
    // Intentar verificar el tipo del token actual más allá del final
    auto result = tokenStream.isCurrentTokenType(TokenType::NUMBER_INT);
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error().type() == ERROR_TYPE::IS_CURRENT_TOKEN_TYPE);
  }

  SECTION("Error en 'insertAfter' con posición inválida") {
    TokenStream newTokens(
        {Token(TokenType::NUMBER_INT, "3", Location{"", 0, 0})});
    // Intentar insertar en una posición inválida
    auto result = tokenStream.insertAfter(newTokens,
                                          std::numeric_limits<int>::infinity());
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error().type() == ERROR_TYPE::INSERT_AFTER);
  }
}