#ifndef PARSER_H
#define PARSER_H

#include "../../lexicalAnalysis/nicoleSintax.h"
#include "../ast/nodeBuilder.h"
#include "../ast/tableBuilder.h"
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