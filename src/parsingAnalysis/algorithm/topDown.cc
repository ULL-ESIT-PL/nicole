#include "../../../inc/parsingAnalysis/algorithm/topDown.h"
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

  if (!root) {
    return std::unexpected{root.error()};
  }

  if (!(*root)) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX, "hola"}};
  }

  const auto tree{Builder::createTree(*root)};

  if (!tree) {
    return std::unexpected{tree.error()};
  }

  return *tree;
}

const std::expected<std::shared_ptr<AST_BODY>, Error>
TopDown::parseStart() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_BODY>, Error>
TopDown::parseBody() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_COMMA>, Error>
TopDown::parseComma() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_STATEMENT>, Error>
TopDown::parseStatement() const noexcept {
  return nullptr;
}

} // namespace nicole