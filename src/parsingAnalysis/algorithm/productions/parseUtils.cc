#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_IMPORT>, Error>
TopDown::parseImport() const noexcept {
  if (!tkStream_.eat()) {
  }

  std::filesystem::path fileName{""};
  if (tkStream_.isCurrentTokenType(TokenType::STRING)) {
    const auto raw{tkStream_.current()->raw()};
    // path of the file that uses import
    std::filesystem::path currentFilePath{
        tkStream_.current()->location().file()};
    // Gets the base path of the current file
    currentFilePath = currentFilePath.parent_path();

    // builds the path based on the import in the current file's directory
    fileName = currentFilePath / raw.substr(1, raw.size() - 2);
  } else {
    // ERROR
  }
  if (!std::filesystem::exists(fileName)) {
    // error
  }
  if (!parsedFiles_.count(fileName)) {
    const auto tokens{lexer_.analyze(fileName)};
    if (!tokens) {
    }
    const auto insertErr{tkStream_.insertAfter(*tokens)};
    if (!insertErr) {
    }
  }
  return Builder::createImport(fileName);
}

const std::expected<std::shared_ptr<AST_PRINT>, Error>
TopDown::parsePrint() const noexcept {
  if (!tkStream_.eat()) {
  }

  if (tkStream_.current()->type() == TokenType::LP) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  if (tkStream_.current()->type() == TokenType::RP) {
    // error
  }

  auto expressions{parseComma()};

  if (tkStream_.current()->type() == TokenType::RP) {
    if (!tkStream_.eat()) {
    }
  } else {
    // error
  }
  return Builder::createPrint(*expressions);
}

} // namespace nicole