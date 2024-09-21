#ifndef PARSER_H
#define PARSER_H

#include "../../lexicalAnalysis/nicoleSintax.h"
#include "../ast/calls/variableCall.h"
#include "../ast/declaration/varDeclaration.h"
#include "../ast/declaration/varTable.h"
#include "../ast/declaration/varReassignment.h"
#include "../ast/declaration/constDeclaration.h"
#include "../ast/literals/nodeLiteralBool.h"
#include "../ast/literals/nodeLiteralChar.h"
#include "../ast/literals/nodeLiteralDouble.h"
#include "../ast/literals/nodeLiteralInt.h"
#include "../ast/literals/nodeLiteralString.h"
#include "../ast/operations/nodeBinaryOp.h"
#include "../ast/statements/statement.h"
#include "../ast/statements/statementList.h"
#include "../ast/conditionals/NodeIfStatetement.h"
#include "tree.h"

namespace nicole {

class Parser {
 protected:
  Lexer lexer_;
  mutable std::vector<Token> tokens_{};
  mutable std::size_t currentToken_{0};
  mutable std::shared_ptr<VariableTable> globalScope_{};

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