#include "../../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <memory>

namespace nicole {

std::shared_ptr<NodeWhileStatement>
TopDown::parseWhileStatement(std::shared_ptr<VariableTable> currentScope,
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
    const std::string strErr{"Error: empty while condition, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto condition{parseLogicalOr(currentScope, father)};

  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto whileScope{TBBuilder::createScope(currentScope)};
  auto whileBody{parseBody(whileScope, father)};
  return ASTBuilder::createWhile(condition, whileBody);
}

std::shared_ptr<NodeDoWhileStatement>
TopDown::parseDoWhileStatement(std::shared_ptr<VariableTable> currentScope,
                               std::shared_ptr<Node> father) const {
  tkStream_.eat();
  auto bodyScope{TBBuilder::createScope(currentScope)};
  auto body{parseBody(bodyScope, father)};
  if (tkStream_.current().type() == TokenType::WHILE) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing while, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::LP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::RP) {
    const std::string strErr{"Error: missing condition, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto condition{parseLogicalOr(currentScope, father)};
  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  return ASTBuilder::createDoWhile(condition, body, father);
}

std::shared_ptr<NodeForStatement>
TopDown::parseForStatement(std::shared_ptr<VariableTable> currentScope,
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
    const std::string strErr{"Error: empty for, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto forScope{TBBuilder::createScope(currentScope)};
  auto init{parseComma(forScope, father)};
  if (tkStream_.current().type() == TokenType::SEMICOLON) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing \";\" to separate init from "
                             "condition in for statement, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto condition{parseLogicalOr(forScope, father)};
  if (tkStream_.current().type() == TokenType::SEMICOLON) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing \";\" to separate condition from "
                             "update in for statement, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto update{parseComma(forScope, father)};
  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto body{parseBody(forScope, father)};
  return ASTBuilder::createFor(init, condition, update, body);
}

}