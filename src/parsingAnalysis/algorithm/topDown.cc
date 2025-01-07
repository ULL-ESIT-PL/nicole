#include "../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <memory>
#include <vector>

namespace nicole {

const std::expected<std::shared_ptr<Tree>, std::vector<Error>>
TopDown::parse(const std::filesystem::path &entryFile) const noexcept {
  parsedFiles_.insert(entryFile);
  const auto tkStream{lexer_.analyze(entryFile)};
  if (!tkStream) {
    erros_.push_back(tkStream.error());
    return std::unexpected<std::vector<Error>>{erros_};
  }

  const std::expected<std::shared_ptr<AST_BODY>, Error> root{parseStart()};

  if (!root) {
    erros_.push_back(root.error());
    return std::unexpected<std::vector<Error>>{erros_}; 
  }
  const auto tree{Builder::createTree((*root))};

  if (!tree) {
    erros_.push_back(tree.error());
    return std::unexpected<std::vector<Error>>{erros_}; 
  }

  return *tree;
}

const std::expected<std::shared_ptr<AST_BODY>, Error>
TopDown::parseStart() const noexcept {
  std::vector<std::shared_ptr<AST>> globalStatements{};
  while (tkStream_.currentPos() < tkStream_.size()) {
  
  }
}

const std::expected<std::shared_ptr<AST_BODY>, Error>
TopDown::parseBody() const noexcept {}

const std::expected<std::shared_ptr<AST_COMMA>, Error>
TopDown::parseComma() const noexcept {}

const std::expected<std::shared_ptr<AST_STATEMENT>, Error>
TopDown::parseStatement() const noexcept {}

} // namespace nicole