#ifndef PARSER_H
#define PARSER_H

#include "../../lexicalAnalysis/sintax.h"
#include "../builder.h"
#include "../ast/tree.h"
#include <set>

namespace nicole {

class Parser {
private:
  Lexer lexer_;
  TokenStream tkStream_{{}};
  std::set<std::filesystem::path> parsedFiles_{};

public:
  explicit Parser(const std::shared_ptr<Sintax> &sintax) noexcept
      : lexer_{sintax->createLexer()} {}

  virtual ~Parser() noexcept = default;

  [[nodiscard]] virtual const std::expected<std::shared_ptr<Tree>, Error>
  parse(const std::filesystem::path &entryFile) const noexcept = 0;
};

} // namespace nicole

#endif // AST_H
