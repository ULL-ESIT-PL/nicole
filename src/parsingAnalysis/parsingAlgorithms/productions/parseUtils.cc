#include "../../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <filesystem>
#include <memory>

namespace nicole {

std::shared_ptr<NodeImport>
TopDown::parseImport(std::shared_ptr<VariableTable> &currentScope,
                     std::shared_ptr<Node> father) const {
  tkStream_.eat();
  std::filesystem::path fileName{""};
  if (tkStream_.isCurrentTokenType(TokenType::STRING)) {
    const auto raw{tkStream_.current().raw()};
    // path of the file that uses import
    std::filesystem::path currentFilePath{
        tkStream_.current().location().file()};
    // Gets the base path of the current file
    currentFilePath = currentFilePath.parent_path();

    // builds the path based on the import in the current file's directory
    fileName = currentFilePath / raw.substr(1, raw.size() - 2);
  } else {
    const std::string strErr{
        "Error missing path after import, found: " + tkStream_.current().raw() +
        " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (!std::filesystem::exists(fileName)) {
    const std::string strErr{
        "Error path " + fileName.string() +
        " not found from: " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (!parsedFiles_.count(fileName)) {
    tkStream_.insertAfter(lexer_.analyze(fileName));
  }
  return ASTBuilder::createImport(fileName, father);
}

std::shared_ptr<NodePrint>
TopDown::parsePrintStatement(std::shared_ptr<VariableTable> currentScope,
                             std::shared_ptr<Node> father) const {
  tkStream_.eat();
  if (tkStream_.current().type() == TokenType::LP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::RP) {
    const std::string strErr{"Error: empty print, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto expressions{parseParamsCall(currentScope, father)};

  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  return ASTBuilder::createPrint(expressions);
}

} // namespace nicole