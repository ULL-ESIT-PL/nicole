#include "../inc/errors.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Pruebas de la clase Error", "[error]") {
  using nicole::Error;
  using nicole::ERROR_TYPE;

  SECTION("Constructor y atributos") {
    Error error{ERROR_TYPE::MISSING_ARGUMENTS, "Se necesitan más argumentos"};
    std::ostringstream oss;
    oss << error;

    REQUIRE(oss.str() ==
            "Error MISSING_ARGUMENTS: Se necesitan más argumentos");
  }

  SECTION("Tipos de errores y mensajes asociados") {
    Error errorBinaryName{ERROR_TYPE::MISSING_BINARY_NAME,
                          "Nombre de binario no proporcionado"};
    std::ostringstream ossBinaryName;
    ossBinaryName << errorBinaryName;

    REQUIRE(ossBinaryName.str() ==
            "Error MISSING_BINARY_NAME: Nombre de binario no proporcionado");

    Error errorFileNotFound{ERROR_TYPE::ENTRY_FILE_NOT_FOUND,
                            "Archivo de entrada no encontrado"};
    std::ostringstream ossFileNotFound;
    ossFileNotFound << errorFileNotFound;

    REQUIRE(ossFileNotFound.str() ==
            "Error ENTRY_FILE_NOT_FOUND: Archivo de entrada no encontrado");
  }

  SECTION("Errores de lexer") {
    Error errorUnmatched{ERROR_TYPE::CHECK_UNMATCHED, "Token no reconocido"};
    std::ostringstream ossUnmatched;
    ossUnmatched << errorUnmatched;

    REQUIRE(ossUnmatched.str() == "Error CHECK_UNMATCHED: Token no reconocido");

    Error errorFileExtension{ERROR_TYPE::FILE_EXTENSION,
                             "Extensión de archivo no válida"};
    std::ostringstream ossFileExtension;
    ossFileExtension << errorFileExtension;

    REQUIRE(ossFileExtension.str() ==
            "Error FILE_EXTENSION: Extensión de archivo no válida");
  }

  SECTION("Errores de flujo de tokens") {
    Error errorEat{ERROR_TYPE::EAT, "No se puede consumir el token actual"};
    std::ostringstream ossEat;
    ossEat << errorEat;

    REQUIRE(ossEat.str() == "Error EAT: No se puede consumir el token actual");

    Error errorLookAhead{ERROR_TYPE::LOOK_AHEAD,
                         "No se puede mirar más allá en el flujo"};
    std::ostringstream ossLookAhead;
    ossLookAhead << errorLookAhead;

    REQUIRE(ossLookAhead.str() ==
            "Error LOOK_AHEAD: No se puede mirar más allá en el flujo");
  }
}
