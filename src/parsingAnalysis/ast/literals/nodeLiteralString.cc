#include "../../../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"

namespace nicole {
std::string NodeLiteralString::unEscape(const std::string &str) const {
  std::string result;
  const std::string strNoQuotes{str.substr(1, str.size() - 2)};
  for (size_t i = 0; i < str.length(); ++i) {
    if (strNoQuotes[i] == '\\' &&
        i + 1 < str.length()) { // Detectar una barra invertida
      switch (strNoQuotes[i + 1]) {
      case 'n': // Nueva línea
        result += '\n';
        break;
      case 't': // Tabulación
        result += '\t';
        break;
      case '\\': // Barra invertida
        result += '\\';
        break;
      case '\"': // Comillas
        result += '\"';
        break;
      default: // Si no es un carácter de escape conocido, agregar el carácter
               // tal cual
        result += strNoQuotes[i + 1];
        break;
      }
      i++; // Saltar el siguiente carácter ya que lo procesamos
    } else {
      result += strNoQuotes[i]; // Agregar el carácter actual
    }
  }
  return result;
}

} // namespace nicole