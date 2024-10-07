#include "../../../../inc/parsingAnalysis/ast/literals/nodeLiteralChar.h"

namespace nicole {
char NodeLiteralChar::unEscape(const std::string &str) const {
  const std::string strNoQuotes{str.substr(1, str.size() - 2)};
  if (strNoQuotes.size() == 1) {
    return strNoQuotes[0];
  }
  // Si hay un carácter de escape, comprobar cuál es
  if (strNoQuotes[0] == '\\' && strNoQuotes.size() == 2) {
    switch (strNoQuotes[1]) {
    case 'n':
      return '\n';
    case 't':
      return '\t';
    case '\\':
      return '\\';
    case '\'':
      return '\'';
    case '\"':
      return '\"';
    default:
      return strNoQuotes[1];
    }
  }
  llvm::report_fatal_error("Cannot reach this point");
}

} // namespace nicole