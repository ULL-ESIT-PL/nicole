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

const std::expected<std::shared_ptr<AST_COMMA>, Error>
TopDown::parseComma() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_STATEMENT>, Error>
TopDown::parseStatement() const noexcept {

  switch (tkStream_.current()->type()) {
  case TokenType::IF: {
    const std::expected<std::shared_ptr<AST>, Error> ifStm{parseIf()};
    if (!ifStm || !*ifStm) {
      return std::unexpected{
          ifStm ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                : ifStm.error()};
    }
    const auto statement{Builder::createStatement(*ifStm)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::SWITCH: {
    const std::expected<std::shared_ptr<AST>, Error> switchSTM{parseSwitch()};
    if (!switchSTM || !*switchSTM) {
      return std::unexpected{
          switchSTM ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                    : switchSTM.error()};
    }
    const auto statement{Builder::createStatement(*switchSTM)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::WHILE: {
    const std::expected<std::shared_ptr<AST>, Error> whileSTM{parseWhile()};
    if (!whileSTM || !*whileSTM) {
      return std::unexpected{
          whileSTM ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                   : whileSTM.error()};
    }
    const auto statement{Builder::createStatement(*whileSTM)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::DO: {
    const std::expected<std::shared_ptr<AST>, Error> doWhile{parseDoWhile()};
    if (!doWhile || !*doWhile) {
      return std::unexpected{
          doWhile ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                  : doWhile.error()};
    }
    const auto statement{Builder::createStatement(*doWhile)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::FOR: {
    const std::expected<std::shared_ptr<AST>, Error> forSTM{parseFor()};
    if (!forSTM || !*forSTM) {
      return std::unexpected{
          forSTM ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                 : forSTM.error()};
    }
    const auto statement{Builder::createStatement(*forSTM)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::PASS: {
    const std::expected<std::shared_ptr<AST_PASS>, Error> pass{parsePass()};
    if (!pass || !*pass) {
      return std::unexpected{pass ? Error{ERROR_TYPE::NULL_NODE, "pass is null"}
                                  : pass.error()};
    }
    const auto statement{Builder::createStatement(*pass)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::STOP: {
    const std::expected<std::shared_ptr<AST_STOP>, Error> stop{parseStop()};
    if (!stop || !*stop) {
      return std::unexpected{
          stop ? Error{ERROR_TYPE::NULL_NODE, "factor is null"} : stop.error()};
    }
    const auto statement{Builder::createStatement(*stop)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::FUNCTION: {
    const std::expected<std::shared_ptr<AST>, Error> function{parseFuncDecl()};
    if (!function || !*function) {
      return std::unexpected{
          function ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                   : function.error()};
    }
    const auto statement{Builder::createStatement(*function)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::RETURN: {
    const std::expected<std::shared_ptr<AST>, Error> returnSTM{parseReturn()};
    if (!returnSTM || !*returnSTM) {
      return std::unexpected{
          returnSTM ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                    : returnSTM.error()};
    }
    const auto statement{Builder::createStatement(*returnSTM)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::ENUM: {
    const std::expected<std::shared_ptr<AST>, Error> enumSTM{parseEnum()};
    if (!enumSTM || !*enumSTM) {
      return std::unexpected{
          enumSTM ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                  : enumSTM.error()};
    }
    const auto statement{Builder::createStatement(*enumSTM)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::STRUCT: {
    const std::expected<std::shared_ptr<AST>, Error> structSTM{
        parseStructDecl()};
    if (!structSTM || !*structSTM) {
      return std::unexpected{
          structSTM ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                    : structSTM.error()};
    }
    const auto statement{Builder::createStatement(*structSTM)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::CLASS: {
    const std::expected<std::shared_ptr<AST>, Error> classSTM{parseClassDecl()};
    if (!classSTM || !*classSTM) {
      return std::unexpected{
          classSTM ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                   : classSTM.error()};
    }
    const auto statement{Builder::createStatement(*classSTM)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::IMPORT: {
    const std::expected<std::shared_ptr<AST>, Error> import{parseImport()};
    if (!import || !*import) {
      return std::unexpected{
          import ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                 : import.error()};
    }
    const auto statement{Builder::createStatement(*import)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  case TokenType::PRINT: {
    const std::expected<std::shared_ptr<AST>, Error> print{parsePrint()};
    if (!print || !*print) {
      return std::unexpected{
          print ? Error{ERROR_TYPE::NULL_NODE, "factor is null"}
                : print.error()};
    }
    const auto statement{Builder::createStatement(*print)};
    if (!statement) {
      return std::unexpected{statement.error()};
    }
    return *statement;
  }

  default: {
    const std::expected<std::shared_ptr<AST>, Error> factor{parseVarDecl(false)};
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