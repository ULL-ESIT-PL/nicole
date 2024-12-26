#include "../inc/lexicalAnalysis/nicoleSintax.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("NicoleSintax crea un Lexer con las categorías correctas",
          "[NicoleSintax]") {
  // Crear una instancia de NicoleSintax
  nicole::NicoleSintax sintax;

  // Crear el Lexer utilizando NicoleSintax
  nicole::Lexer lexer = sintax.createLexer();

  // Obtener las categorías del Lexer
  std::vector<nicole::Category> categories = lexer.categories();

  // Verificar que las categorías no estén vacías
  REQUIRE_FALSE(categories.empty());

  // Opcional: Verificar algunas categorías específicas
  bool foundSpaceCategory = false;
  bool foundCommentCategory = false;

  for (const auto &category : categories) {
    if (category.type() == nicole::TokenType::SPACE) {
      foundSpaceCategory = true;
      REQUIRE(category.rawPattern() == "\\s+");
      REQUIRE(category.skip() == true);
    } else if (category.type() == nicole::TokenType::COMMENT) {
      foundCommentCategory = true;
      REQUIRE(category.rawPattern() == "\\/\\*(.|\\n)*?\\*\\/");
      REQUIRE(category.skip() == true);
    }
    // Puedes agregar más verificaciones para otras categorías según sea
    // necesario
  }

  REQUIRE(foundSpaceCategory);
  REQUIRE(foundCommentCategory);
}
