#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <vector>

namespace nicole {

const std::expected<std::shared_ptr<AST_IMPORT>, Error>
TopDown::parseImport() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() != TokenType::STRING) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "missing file of import at " + tkStream_.current()->locInfo()}};
  }
  const Token token{*tkStream_.current()};
  if (!tkStream_.eat()) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "failed to eat " + token.raw() + " at " + token.locInfo()}};
  }
  std::filesystem::path currentFilePath{token.location().file()};
  // Gets the base path of the current file
  currentFilePath = currentFilePath.parent_path();
  // builds the path based on the import in the current file's directory
  currentFilePath =
      currentFilePath / token.raw().substr(1, token.raw().size() - 2);
  if (!parsedFiles_.count(currentFilePath)) {
    parsedFiles_.insert(currentFilePath);
    const std::expected<TokenStream, Error> tokens{
        lexer_.analyze(currentFilePath)};
    if (!tokens) {
      return std::unexpected{tokens.error()};
    }
    const std::expected<std::monostate, Error> isInserted{
        tkStream_.insertAfter(*tokens, tkStream_.currentPos() + 1)};
    if (!isInserted) {
      return std::unexpected{isInserted.error()};
    }
  }
  if (tkStream_.current()->type() != TokenType::SEMICOLON) {
    return std::unexpected{Error{
        ERROR_TYPE::SINTAX, "missing ; at " + tkStream_.current()->locInfo()}};
  }
  return Builder::createImport(currentFilePath);
}

const std::expected<std::shared_ptr<AST_PRINT>, Error>
TopDown::parsePrint() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() != TokenType::LP) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "missing left parenthesis at " + tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() == TokenType::RP) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "empty expression at " + tkStream_.current()->locInfo()}};
  }
  std::vector<std::shared_ptr<AST>> params{};
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::RP) {
    const std::expected<std::shared_ptr<AST>, Error> param{parseOr()};
    if (!param || !*param) {
      return std::unexpected{
          param ? Error{ERROR_TYPE::NULL_NODE, "node is null"} : param.error()};
    }
    params.push_back(*param);
    if (tkStream_.current()->type() == TokenType::COMMA) {
      if (!tkStream_.eat()) {
        return std::unexpected{Error{
            ERROR_TYPE::SINTAX, "failed to eat " + tkStream_.current()->raw() +
                                    " at " + tkStream_.current()->locInfo()}};
      }
      continue;
    } else if (tkStream_.current()->type() != TokenType::RP) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "missing comma or parenthesis at " +
                                        tkStream_.current()->locInfo()}};
    }
    break;
  }
  if (tkStream_.current()->type() != TokenType::RP) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "missing right parenthesis at " +
                                      tkStream_.current()->locInfo()}};
  }
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }
  if (tkStream_.current()->type() != TokenType::SEMICOLON) {
    return std::unexpected{Error{
        ERROR_TYPE::SINTAX, "missing ; at " + tkStream_.current()->locInfo()}};
  }
  return Builder::createPrint(params);
}

} // namespace nicole