#include "../inc/errors.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Pruebas de la clase Error", "[error]") {
  using nicole::Error;
  using nicole::ERROR_TYPE;

  SECTION("Constructor y atributos") {
    const Error error{ERROR_TYPE::MISSING_ARGUMENTS,
                      "Se necesitan más argumentos"};
    std::ostringstream oss;
    oss << error;

    REQUIRE(error.info() == "Se necesitan más argumentos");
    REQUIRE(oss.str() ==
            "Error MISSING_ARGUMENTS: Se necesitan más argumentos");
  }

  SECTION("Tipos de errores y mensajes asociados") {
    const Error errorBinaryName{ERROR_TYPE::MISSING_BINARY_NAME,
                                "Nombre de binario no proporcionado"};
    std::ostringstream ossBinaryName;
    ossBinaryName << errorBinaryName;

    REQUIRE(errorBinaryName.info() == "Nombre de binario no proporcionado");
    REQUIRE(ossBinaryName.str() ==
            "Error MISSING_BINARY_NAME: Nombre de binario no proporcionado");

    const Error errorFileNotFound{ERROR_TYPE::ENTRY_FILE_NOT_FOUND,
                                  "Archivo de entrada no encontrado"};
    std::ostringstream ossFileNotFound;
    ossFileNotFound << errorFileNotFound;

    REQUIRE(errorFileNotFound.info() == "Archivo de entrada no encontrado");
    REQUIRE(ossFileNotFound.str() ==
            "Error ENTRY_FILE_NOT_FOUND: Archivo de entrada no encontrado");
  }

  SECTION("Errores de lexer") {
    const Error errorUnmatched{ERROR_TYPE::CHECK_UNMATCHED,
                               "Token no reconocido"};
    std::ostringstream ossUnmatched;
    ossUnmatched << errorUnmatched;

    REQUIRE(errorUnmatched.info() == "Token no reconocido");
    REQUIRE(ossUnmatched.str() == "Error CHECK_UNMATCHED: Token no reconocido");

    const Error errorFileExtension{ERROR_TYPE::FILE_EXTENSION,
                                   "Extensión de archivo no válida"};
    std::ostringstream ossFileExtension;
    ossFileExtension << errorFileExtension;

    REQUIRE(errorFileExtension.info() == "Extensión de archivo no válida");
    REQUIRE(ossFileExtension.str() ==
            "Error FILE_EXTENSION: Extensión de archivo no válida");
  }

  SECTION("Errores de flujo de tokens") {
    const Error errorEat{ERROR_TYPE::EAT,
                         "No se puede consumir el token actual"};
    std::ostringstream ossEat;
    ossEat << errorEat;

    REQUIRE(errorEat.info() == "No se puede consumir el token actual");
    REQUIRE(ossEat.str() == "Error EAT: No se puede consumir el token actual");

    const Error errorLookAhead{ERROR_TYPE::LOOK_AHEAD,
                               "No se puede mirar más allá en el flujo"};
    std::ostringstream ossLookAhead;
    ossLookAhead << errorLookAhead;

    REQUIRE(errorLookAhead.info() == "No se puede mirar más allá en el flujo");
    REQUIRE(ossLookAhead.str() ==
            "Error LOOK_AHEAD: No se puede mirar más allá en el flujo");
  }
}
