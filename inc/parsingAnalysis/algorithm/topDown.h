#ifndef TOP_DOWN_H
#define TOP_DOWN_H

#include "parser.h"
#include <memory>

namespace nicole {

class TopDown final : public Parser {
private:
  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseStart() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseBody() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_COMMA>, Error>
  parseComma() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_STATEMENT>, Error>
  parseStatement() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_ENUM>, Error>
  parseEnum() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_CLASS>, Error>
  parseClassDecl() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_STRUCT>, Error>
  parseStructDecl() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
  parseFuncDecl() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_RETURN>, Error>
  parseReturn() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_IMPORT>, Error>
  parseImport() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_PRINT>, Error>
  parsePrint() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_WHILE>, Error>
  parseWhile() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_FOR>, Error>
  parseFor() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_DO_WHILE>, Error>
  parseDoWhile() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_IF>, Error>
  parseIf() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_ELSE_IF>, Error>
  parseElseIf() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_SWITCH>, Error>
  parseSwitch() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_CASE>, Error>
  parseCase() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_DEFAULT>, Error>
  parseDefault() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseSelfAssigment() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseVarDecl() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseTernary() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseAnd() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseOr() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseEqual_NotEqual() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseCompare() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseAdd_Sub() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST>, Error>
  parseMult_Div_Module() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST>, Error>
  parseFactor() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseVector() const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<AST_BODY>, Error>
  parseParams() const noexcept;

public:
  explicit TopDown(const std::shared_ptr<Sintax> &sintax) noexcept
      : Parser{sintax} {}

  [[nodiscard]] const std::expected<std::shared_ptr<Tree>, Error>
  parse(const std::filesystem::path &entryFile) const noexcept override;
};

} // namespace nicole

#endif