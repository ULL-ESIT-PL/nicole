#include "../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <filesystem>
#include <memory>

namespace nicole {

std::shared_ptr<Tree> TopDown::parse(const std::filesystem::path &path) const {
  tkStream_ = lexer_.analyze(path, false);
  parsedFiles_.insert(path);
  globalScope_ = TBBuilder::createScope(nullptr);
  typeTable_ = TBBuilder::createTypeTB();
  functionTable_ = TBBuilder::createFunctTB();
  root_ = parseStart();

  return std::make_shared<Tree>(root_);
}

std::shared_ptr<NodeStatementList> TopDown::parseStart() const {
  std::vector<std::shared_ptr<NodeStatement>> gloablScopeStatements{};
  std::shared_ptr<Node> father{nullptr};
  while (tkStream_.currentPos() < tkStream_.size()) {
    gloablScopeStatements.push_back(parseStatement(globalScope_, father));
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
      tkStream_.eat();
    }
  }

  return ASTBuilder::createList(gloablScopeStatements);
}

std::shared_ptr<NodeStatementList>
TopDown::parseBody(std::shared_ptr<VariableTable> &bodyScope,
                   std::shared_ptr<Node> father) const {
  if (tkStream_.current().type() == TokenType::LB) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left bracket, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  std::vector<std::shared_ptr<NodeStatement>> body;
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::RB)) {
    body.push_back(parseStatement(bodyScope, father));
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
      tkStream_.eat();
    }
  }
  tkStream_.eat(); // Consume el token "}"
  return ASTBuilder::createList(body);
}

std::shared_ptr<NodeStatementList>
TopDown::parseComma(std::shared_ptr<VariableTable> &currentScope,
                    std::shared_ptr<Node> father) const {
  std::vector<std::shared_ptr<NodeStatement>> body;
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    body.push_back(
        ASTBuilder::createStatement(parseVarDeclaration(currentScope, father)));
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::COMMA)) {
      tkStream_.eat();
    } else {
      break;
    }
  }
  return ASTBuilder::createList(body);
}

std::shared_ptr<ParamsDeclaration>
TopDown::parseParams(std::shared_ptr<VariableTable> &currentScope,
                     std::shared_ptr<Node> father) const {
  std::vector<std::pair<std::string, std::shared_ptr<GenericType>>> params{};
  if (tkStream_.current().type() == TokenType::RP) {
    return std::make_shared<ParamsDeclaration>(params, currentScope);
  }
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    auto token{tkStream_.current()};
    std::string id{};
    std::shared_ptr<GenericType> type{nullptr};
    if (token.type() == TokenType::ID) {
      id = token.raw();
      tkStream_.eat();
    } else {
      const std::string strErr{"Expected parameter, found: " + token.raw() +
                               " at " + token.locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
    if (tkStream_.current().type() == TokenType::DOTDOT) {
      tkStream_.eat();
    } else {
      const std::string strErr{
          "Expected :, found: " + tkStream_.current().raw() + " at " +
          tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
    token = tkStream_.current();
    if (token.type() == TokenType::ID) {
      type = typeTable_->type(token.raw());
      tkStream_.eat();
    } else {
      const std::string strErr{"Expected type, found: " + token.raw() + " at " +
                               token.locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
    params.push_back({id, type});
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::COMMA)) {
      tkStream_.eat();
    } else {
      break;
    }
  }
  return std::make_shared<ParamsDeclaration>(params, currentScope);
}

std::vector<std::shared_ptr<Node>>
TopDown::parseParamsCall(std::shared_ptr<VariableTable> &currentScope,
                         std::shared_ptr<Node> father) const {
  std::vector<std::shared_ptr<Node>> params{};
  while (tkStream_.currentPos() < tkStream_.size() &&
         !tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    params.push_back(parseLogicalOr(currentScope, father));
    if (tkStream_.currentPos() < tkStream_.size() &&
        tkStream_.isCurrentTokenType(TokenType::COMMA)) {
      tkStream_.eat();
    } else {
      break;
    }
  }
  return params;
}

std::shared_ptr<NodeStatement>
TopDown::parseStatement(std::shared_ptr<VariableTable> currentScope,
                        std::shared_ptr<Node> father) const {
  if (tkStream_.isCurrentTokenType(TokenType::ID) &&
      (tkStream_.lookAhead(1).type() == TokenType::SELF_ADD ||
       tkStream_.lookAhead(1).type() == TokenType::SELF_SUB ||
       tkStream_.lookAhead(1).type() == TokenType::SELF_MULT ||
       tkStream_.lookAhead(1).type() == TokenType::SELF_DIV)) {
    return ASTBuilder::createStatement(
        parseSelfAssignment(currentScope, father));
  }
  switch (tkStream_.current().type()) {
  case TokenType::SWITCH: {
    return ASTBuilder::createStatement(
        parseSwitchStatement(currentScope, father), father);
  }
  case TokenType::IF: {
    return ASTBuilder::createStatement(parseIfStatement(currentScope, father),
                                       father);
  }
  case TokenType::WHILE: {
    return ASTBuilder::createStatement(
        parseWhileStatement(currentScope, father), father);
  }
  case TokenType::DO: {
    return ASTBuilder::createStatement(
        parseDoWhileStatement(currentScope, father), father);
  }
  case TokenType::FOR: {
    return ASTBuilder::createStatement(parseForStatement(currentScope, father),
                                       father);
  }
  case TokenType::PRINT: {
    return ASTBuilder::createStatement(
        parsePrintStatement(currentScope, father), father);
  }
  case TokenType::STRUCT: {
    return ASTBuilder::createStatement(
        parseStructDeclaration(currentScope, father), father);
  }
  case TokenType::FUNCTION: {
    return ASTBuilder::createStatement(
        parseFunctionDeclaration(currentScope, father), father);
  }
  case TokenType::RETURN: {
    return ASTBuilder::createStatement(parseReturn(currentScope, father),
                                       father);
  }
  case TokenType::IMPORT: {
    return ASTBuilder::createStatement(parseImport(currentScope, father),
                                       father);
  }
  default: {
    return ASTBuilder::createStatement(
        parseVarDeclaration(currentScope, father), father);
  }
  }
}

} // namespace nicole
