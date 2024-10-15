#ifndef PARSER_H
#define PARSER_H

#include "../../lexicalAnalysis/nicoleSintax.h"
#include "../ast/nodeBuilder.h"
#include "../ast/tableBuilder.h"
#include "tree.h"
#include <filesystem>
#include <memory>
#include <set>

namespace nicole {

// Abstract class that represents a parser so we can switch between parsers
class Parser {
protected:
  Lexer lexer_;
  mutable TokenStream tkStream_{{}};
  mutable std::shared_ptr<VariableTable> globalScope_{};
  mutable std::shared_ptr<TypeTable> typeTable_{};
  mutable std::shared_ptr<FunctionTable> functionTable_{};
  mutable std::set<std::filesystem::path> parsedFiles_{};

public:
  Parser(std::shared_ptr<Sintax> sintax) : lexer_{sintax->createLexer()} {}

  virtual ~Parser() = default;

  virtual std::shared_ptr<Tree>
  parse(const std::filesystem::path &path) const = 0;
};

} // namespace nicole

#endif