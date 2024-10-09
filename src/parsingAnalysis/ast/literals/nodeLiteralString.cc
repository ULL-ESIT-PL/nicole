#include "../../../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"

namespace nicole {
std::string NodeLiteralString::unEscape(const std::string &str) const {
  std::string result;
  const std::string strNoQuotes{str.substr(1, str.size() - 2)};
  for (size_t i = 0; i < str.length(); ++i) {
    if (strNoQuotes[i] == '\\' && i + 1 < str.length()) {
      switch (strNoQuotes[i + 1]) {
      case 'n':
        result += '\n';
        break;
      case 't':
        result += '\t';
        break;
      case '\\':
        result += '\\';
        break;
      case '\"':
        result += '\"';
        break;
      default:
        result += strNoQuotes[i + 1];
        break;
      }
      i++;
    } else {
      result += strNoQuotes[i]; // Agregar el carácter actual
    }
  }
  return result;
}

} // namespace nicole