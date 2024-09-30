#ifndef PARSER_H
#define PARSER_H

#include "../../lexicalAnalysis/nicoleSintax.h"
#include "../ast/calls/functionCall.h"
#include "../ast/calls/structConstructor.h"
#include "../ast/calls/variableCall.h"
#include "../ast/conditionals/nodeIfStatement.h"
#include "../ast/declaration/constDeclaration.h"
#include "../ast/declaration/functionTable.h"
#include "../ast/declaration/nodeFunDeclaration.h"
#include "../ast/declaration/nodeReturn.h"
#include "../ast/declaration/paramsDeclaration.h"
#include "../ast/declaration/structDeclaration.h"
#include "../ast/declaration/varDeclaration.h"
#include "../ast/declaration/varReassignment.h"
#include "../ast/declaration/varTable.h"
#include "../ast/literals/nodeLiteralBool.h"
#include "../ast/literals/nodeLiteralChar.h"
#include "../ast/literals/nodeLiteralFloat.h"
#include "../ast/literals/nodeLiteralInt.h"
#include "../ast/literals/nodeLiteralString.h"
#include "../ast/loops/nodeForStatement.h"
#include "../ast/loops/nodePass.h"
#include "../ast/loops/nodeStop.h"
#include "../ast/loops/nodeWhileStatement.h"
#include "../ast/operations/nodeBinaryOp.h"
#include "../ast/operations/nodeIncrement.h"
#include "../ast/operations/nodeUnaryOp.h"
#include "../ast/statements/statement.h"
#include "../ast/statements/statementList.h"
#include "../ast/utils/nodePrint.h"
#include "../types/typeTable.h"
#include "tree.h"
#include <memory>

namespace nicole {

class Parser {
protected:
  Lexer lexer_;
  mutable std::vector<Token> tokens_{};
  mutable std::size_t currentToken_{0};
  mutable std::shared_ptr<VariableTable> globalScope_{};
  mutable std::shared_ptr<TypeTable> typeTable_{};
  mutable std::shared_ptr<FunctionTable> functionTable_{};

  void eat() const;

  Token getCurrentToken() const;

  bool isTokenType(const TokenType &type) const;

public:
  Parser(std::shared_ptr<Sintax> sintax) : lexer_{sintax->createLexer()} {}

  virtual ~Parser() = default;

  virtual std::shared_ptr<Tree>
  parse(const std::filesystem::path &path) const = 0;
};

} // namespace nicole

#endif