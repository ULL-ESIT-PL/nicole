#include <catch2/catch_test_macros.hpp>
#include "../inc/lexicalAnalysis/nicoleSintax.h"

TEST_CASE("NicoleSintax creates a valid Lexer", "[NicoleSintax]") {
    // Instancia de NicoleSintax
    nicole::NicoleSintax sintax;

    // Crear el Lexer
    nicole::Lexer lexer = sintax.createLexer();

    // Verificar que las categorías del Lexer no estén vacías
    REQUIRE_FALSE(lexer.categories().empty());

    // Ruta al archivo fuente de prueba
    std::filesystem::path testFile = "ruta/al/archivo_de_prueba.nc";

    // Analizar el archivo fuente
    auto result = lexer.analyze(testFile);

    // Verificar que el análisis fue exitoso
    REQUIRE(result.has_value());

    // Verificar que el TokenStream no esté vacío
    REQUIRE_FALSE(result->size());
}
