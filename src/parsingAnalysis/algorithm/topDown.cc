#include "../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <cstddef>
#include <expected>
#include <memory>
#include <ostream>
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
  const std::size_t size{tkStream_.size()};
  std::vector<std::shared_ptr<AST_STATEMENT>> statements{};

  while (tkStream_.currentPos() < size) {
    auto statement = parseStatement();
    if (!statement || !*statement) {
      return std::unexpected{
          statement ? Error{ERROR_TYPE::NULL_NODE, "Statement is null"}
                    : statement.error()};
    }

    statements.push_back(*statement);

    if (tkStream_.isCurrentTokenType(TokenType::SEMICOLON) &&
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
  if (!(tkStream_.current()->type() == TokenType::LB)) {
    return std::unexpected{Error{
        ERROR_TYPE::SINTAX, "missing { at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }

  const std::size_t size{tkStream_.size()};
  std::vector<std::shared_ptr<AST_STATEMENT>> statements{};

  while (tkStream_.currentPos() < size) {
    auto statement = parseStatement();
    if (!statement || !*statement) {
      return std::unexpected{
          statement ? Error{ERROR_TYPE::NULL_NODE, "Statement is null"}
                    : statement.error()};
    }

    statements.push_back(*statement);

    if (tkStream_.isCurrentTokenType(TokenType::SEMICOLON) &&
        !tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "Failed to consume semicolon"}};
    }
  }

  if (!(tkStream_.current()->type() == TokenType::RB)) {
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

const std::expected<std::shared_ptr<AST_COMMA>, Error>
TopDown::parseComma() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_STATEMENT>, Error>
TopDown::parseStatement() const noexcept {

  switch (tkStream_.current()->type()) {
  case TokenType::IF: {
  }

  case TokenType::SWITCH: {
  }

  case TokenType::WHILE: {
  }

  case TokenType::DO: {
  }

  case TokenType::FOR: {
  }

  case TokenType::FUNCTION: {
  }

  case TokenType::RETURN: {
  }

  case TokenType::ENUM: {
  }

  case TokenType::STRUCT: {
  }

  case TokenType::CLASS: {
  }

  case TokenType::IMPORT: {
  }

  case TokenType::PRINT: {
  }

  default: {
    const std::expected<std::shared_ptr<AST>, Error> factor{parseVarDecl()};
    if (!factor || !*factor) {
      return std::unexpected{
          factor ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                 : factor.error()};
    }

    const auto statement{Builder::createStatement(*factor)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }
  }
}

} // namespace nicole