#ifndef PARSER_H
#define PARSER_H

#include "../lexer/nicoleSintax.h"
#include "literals/nodeLiteralBool.h"
#include "literals/nodeLiteralChar.h"
#include "literals/nodeLiteralDouble.h"
#include "literals/nodeLiteralInt.h"
#include "literals/nodeLiteralString.h"
#include "operations/nodeBinary.h"

namespace nicole {
class Parser {
 private:
  Lexer lexer{NicoleSintax::createLexer()};
  mutable std::vector<Token> tokens_{};

 public:
  void parse(const std::filesystem::path& path) const;
};
}  // namespace nicole

#endif