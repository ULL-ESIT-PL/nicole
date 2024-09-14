#ifndef PARSER_H
#define PARSER_H

#include "../..//lexicalAnalysis/nicoleSintax.h"
#include "../declaration/varDeclaration.h"
#include "../declaration/varTable.h"
#include "../literals/nodeLiteralBool.h"
#include "../literals/nodeLiteralChar.h"
#include "../literals/nodeLiteralDouble.h"
#include "../literals/nodeLiteralInt.h"
#include "../literals/nodeLiteralString.h"
#include "../operations/nodeBinaryOp.h"
#include "../statements/statement.h"
#include "../statements/statementList.h"
#include "tree.h"

namespace nicole {

class Parser {
 protected:
  Lexer lexer_;
  mutable std::vector<Token> tokens_{};
  mutable std::size_t currentToken_{0};
  mutable std::unique_ptr<VariableTable> globalScope_{};

  void eat() const;

  Token getCurrentToken() const;

  bool isTokenType(const TokenType& type) const;

 public:
  Parser(std::unique_ptr<Sintax> sintax) : lexer_{sintax->createLexer()} {}

  virtual ~Parser() = default;

  virtual std::unique_ptr<Tree> parse(
      const std::filesystem::path& path) const = 0;
};

}  // namespace nicole

#endif