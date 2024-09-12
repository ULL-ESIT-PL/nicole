#ifndef PARSER_H
#define PARSER_H

#include "../..//lexicalAnalysis/nicoleSintax.h"
#include "../literals/nodeLiteralBool.h"
#include "../literals/nodeLiteralChar.h"
#include "../literals/nodeLiteralDouble.h"
#include "../literals/nodeLiteralInt.h"
#include "../literals/nodeLiteralString.h"
#include "../operations/nodeBinaryOp.h"
#include "../statements/statement.h"

namespace nicole {
class Parser {
 protected:
  Lexer lexer_;
  mutable std::vector<Token> tokens_{};
  mutable std::size_t currentToke_{0};

  void eat() const;
  Token getCurrentToke() const;

 public:
  Parser(std::unique_ptr<Sintax> sintax) : lexer_{sintax->createLexer()} {}

  virtual ~Parser() = default;

  virtual std::unique_ptr<Node> parse(
      const std::filesystem::path& path) const = 0;
};
}  // namespace nicole

#endif