#include "../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <expected>
#include <memory>
#include <vector>

namespace nicole {

const std::expected<std::shared_ptr<Tree>, Error>
TopDown::parse(const std::filesystem::path &entryFile) const noexcept {
  parsedFiles_.insert(entryFile);

  const auto tkStream{lexer_.analyze(entryFile)};

  if (!tkStream) {
    return std::unexpected{tkStream.error()};
  }

  tkStream_ = *tkStream;

  const std::expected<std::shared_ptr<AST_BODY>, Error> root{parseStart()};

  if (!root || !*root) {
    return std::unexpected{root ? Error{ERROR_TYPE::NULL_NODE, "Tree is null"}
                                : root.error()};
  }

  const auto tree{Builder::createTree(*root)};

  if (!tree) {
    return std::unexpected{tree.error()};
  }

  return *tree;
}

const std::expected<std::shared_ptr<AST_BODY>, Error>
TopDown::parseStart() const noexcept {
  std::vector<std::shared_ptr<AST_STATEMENT>> statements{};

  while (tkStream_.currentPos() < tkStream_.size()) {
    const std::expected<std::shared_ptr<AST_STATEMENT>, Error> statement{
        parseStatement()};
    if (!statement || !*statement) {
      return std::unexpected{
          statement ? Error{ERROR_TYPE::NULL_NODE, "Statement is null"}
                    : statement.error()};
    }
    statements.push_back(*statement);
    if (tkStream_.current()->type() == TokenType::SEMICOLON &&
        !tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "Failed to consume semicolon"}};
    }
  }

  const std::expected<std::shared_ptr<AST_BODY>, Error> body{
      Builder::createBody(statements)};
  if (!body) {
    return std::unexpected{body.error()};
  }

  return body;
}

const std::expected<std::shared_ptr<AST_BODY>, Error>
TopDown::parseBody() const noexcept {
  if (tkStream_.current()->type() != TokenType::LB) {
    return std::unexpected{Error{
        ERROR_TYPE::SINTAX, "missing { at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }

  std::vector<std::shared_ptr<AST_STATEMENT>> statements{};

  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::RB) {
    const std::expected<std::shared_ptr<AST_STATEMENT>, Error> statement{
        parseStatement()};
    if (!statement || !*statement) {
      return std::unexpected{
          statement ? Error{ERROR_TYPE::NULL_NODE, "Statement is null"}
                    : statement.error()};
    }
    statements.push_back(*statement);
    if (tkStream_.current()->type() == TokenType::SEMICOLON &&
        !tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "Failed to consume semicolon"}};
    }
  }
  if (tkStream_.current()->type() != TokenType::RB) {
    return std::unexpected{Error{
        ERROR_TYPE::SINTAX, "missing } at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }

  const std::expected<std::shared_ptr<AST_BODY>, Error> body{
      Builder::createBody(statements)};
  if (!body) {
    return std::unexpected{body.error()};
  }

  return body;
}

const std::expected<std::shared_ptr<AST_STATEMENT>, Error>
TopDown::parseStatement() const noexcept {

  std::expected<std::shared_ptr<AST>, Error> statement{nullptr};

  if ((tkStream_.isTokenAheadBeforeSemicolon(TokenType::DOTDOT_ASSIGNMENT)) or
      tkStream_.isTokenAheadBeforeSemicolon(TokenType::SELF_ADD) or
      tkStream_.isTokenAheadBeforeSemicolon(TokenType::SELF_SUB) or
      tkStream_.isTokenAheadBeforeSemicolon(TokenType::SELF_MULT) or
      tkStream_.isTokenAheadBeforeSemicolon(TokenType::SELF_DIV)) {
    statement = parseSelfAssignment(false);
    if (!statement || !*statement) {
      return std::unexpected{statement
                                 ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : statement.error()};
    }

    return Builder::createStatement(*statement);
  }

  switch (tkStream_.current()->type()) {
  case TokenType::IF: {
    statement = parseIf();
    break;
  }

  case TokenType::SWITCH: {
    statement = parseSwitch();
    break;
  }

  case TokenType::WHILE: {
    statement = parseWhile();
    break;
  }

  case TokenType::DO: {
    statement = parseDoWhile();
    break;
  }

  case TokenType::FOR: {
    statement = parseFor();
    break;
  }

  case TokenType::PASS: {
    statement = parsePass();
    break;
  }

  case TokenType::STOP: {
    statement = parseStop();
    break;
  }

  case TokenType::FUNCTION: {
    statement = parseFuncDecl(false);
    break;
  }

  case TokenType::RETURN: {
    statement = parseReturn();
    break;
  }

  case TokenType::ENUM: {
    statement = parseEnum();
    break;
  }

  case TokenType::STRUCT: {
    statement = parseStructDecl();
    break;
  }

  case TokenType::CLASS: {
    statement = parseClassDecl();
    break;
  }

  case TokenType::DELETE: {
    statement = parseDelete();
    break;
  }

  case TokenType::IMPORT: {
    statement = parseImport();
    break;
  }

  case TokenType::PRINT: {
    statement = parsePrint();
    break;
  }

  default: {
    statement = parseVarDecl(false);
    break;
  }
  }

  if (!statement || !*statement) {
    return std::unexpected{statement
                               ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                               : statement.error()};
  }
  return Builder::createStatement(*statement);
}

} // namespace nicole