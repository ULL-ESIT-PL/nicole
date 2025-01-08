#include "../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <expected>
#include <memory>
#include <vector>

namespace nicole {

const std::expected<std::shared_ptr<Tree>, std::vector<Error>>
TopDown::parse(const std::filesystem::path &entryFile) const noexcept {
  parsedFiles_.insert(entryFile);
  const auto tkStream{lexer_.analyze(entryFile)};
  if (!tkStream) {
  }

  const std::expected<std::shared_ptr<AST_BODY>, Error> root{parseStart()};

  if (!root) {
  }
  const auto tree{Builder::createTree((*root))};

  if (!tree) {
  }

  return *tree;
}

const std::expected<std::shared_ptr<AST_BODY>, Error>
TopDown::parseStart() const noexcept {
  std::vector<std::shared_ptr<AST_STATEMENT>> globalStatements{};

  while (tkStream_.currentPos() < tkStream_.size()) {
    const auto statement{parseStatement()};
    if (!statement) {
    }
    globalStatements.push_back(*statement);

    const auto isType{tkStream_.isCurrentTokenType(TokenType::SEMICOLON)};
    if (!isType) {
    }
    if (tkStream_.currentPos() < tkStream_.size() and isType) {
      const auto eaten{tkStream_.eat()};
      if (!eaten) {
      }
    }
  }
  const auto body{Builder::createBody(globalStatements)};
  if (!body) {
  }
  return body;
}

const std::expected<std::shared_ptr<AST_BODY>, Error>
TopDown::parseBody() const noexcept {
  const auto current{tkStream_.current()};
  if (!current) {
  }

  if (current->type() == TokenType::LB) {
    const auto eaten{tkStream_.eat()};
    if (!eaten) {
    }
  } else {
    // error
  }
  std::vector<std::shared_ptr<AST_STATEMENT>> statements;
  while (tkStream_.currentPos() < tkStream_.size() &&
         !(*tkStream_.isCurrentTokenType(TokenType::RB))) {
    const auto node{parseStatement()};
    if (!node) {
    }
    statements.push_back(*node);

    const auto isType{tkStream_.isCurrentTokenType(TokenType::SEMICOLON)};
    if (!isType) {
    }

    if (tkStream_.currentPos() < tkStream_.size() && isType) {
      const auto eaten{tkStream_.eat()};
      if (!eaten) {
      }
    }
  }
  const auto eaten{tkStream_.eat()}; // Consume el token "}"
  if (!eaten) {
  }

  const auto body{Builder::createBody(statements)};
  if (!body) {
  }

  return *body;
}

const std::expected<std::shared_ptr<AST_COMMA>, Error>
TopDown::parseComma() const noexcept {
  std::vector<std::shared_ptr<AST_STATEMENT>> body;
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {

    const auto node{parseVarDecl()};

    if (!node) {
    }

    const auto statement{Builder::createStatement(*node)};
    if (!statement) {
    }

    body.push_back(*statement);
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::COMMA)) {
      const auto eaten{tkStream_.eat()};
      if (!eaten) {
      }
    } else {
      break;
    }
  }
  return Builder::createCOMMA(body);
}

const std::expected<std::shared_ptr<AST_STATEMENT>, Error>
TopDown::parseStatement() const noexcept {
  const auto ahead{tkStream_.lookAhead(1)};

  if (!ahead) {
  }

  if (tkStream_.isCurrentTokenType(TokenType::ID) &&
      (ahead->type() == TokenType::SELF_ADD ||
       ahead->type() == TokenType::SELF_SUB ||
       ahead->type() == TokenType::SELF_MULT ||
       ahead->type() == TokenType::SELF_DIV)) {

    const auto astSelf{parseSelfAssignment()};

    if (!astSelf) {
    }

    return Builder::createStatement(*astSelf);
  }

  const auto current{tkStream_.current()};

  if (!current) {
  }

  switch (current->type()) {
  case TokenType::SWITCH: {
    const auto astSwitch{parseSwitch()};

    if (!astSwitch) {
    }

    return Builder::createStatement(*astSwitch);
  }
  case TokenType::IF: {
    const auto astIf{parseIf()};

    if (!astIf) {
    }

    return Builder::createStatement(*astIf);
  }
  case TokenType::WHILE: {
    const auto astWhile{parseWhile()};

    if (!astWhile) {
    }

    return Builder::createStatement(*astWhile);
  }
  case TokenType::DO: {
    const auto astDoWhile{parseDoWhile()};

    if (!astDoWhile) {
    }

    return Builder::createStatement(*astDoWhile);
  }
  case TokenType::FOR: {
    const auto astFor{parseFor()};

    if (!astFor) {
    }

    return Builder::createStatement(*astFor);
  }
  case TokenType::PRINT: {
    const auto astPrint{parsePrint()};

    if (!astPrint) {
    }

    return Builder::createStatement(*astPrint);
  }
  case TokenType::STRUCT: {
    const auto astStruct{parseStructDecl()};

    if (!astStruct) {
    }

    return Builder::createStatement(*astStruct);
  }
  case TokenType::FUNCTION: {
    const auto astFunctionDecl{parseFuncDecl()};

    if (!astFunctionDecl) {
    }

    return Builder::createStatement(*astFunctionDecl);
  }
  case TokenType::RETURN: {
    const auto astReturn{parseReturn()};

    if (!astReturn) {
    }

    return Builder::createStatement(*astReturn);
  }
  case TokenType::IMPORT: {
    const auto astImport{parseImport()};

    if (!astImport) {
    }

    return Builder::createStatement(*astImport);
  }
  default: {
    const auto astVarDecl{parseVarDecl()};

    if (!astVarDecl) {
    }

    return Builder::createStatement(*astVarDecl);
  }
  }
}

} // namespace nicole