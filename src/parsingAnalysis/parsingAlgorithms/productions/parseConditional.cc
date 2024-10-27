#include "../../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <memory>
#include <vector>

namespace nicole {

std::shared_ptr<NodeSwitchStatement>
TopDown::parseSwitchStatement(std::shared_ptr<VariableTable> currentScope,
                              std::shared_ptr<Node> father) const {
  tkStream_.eat();
  if (tkStream_.isCurrentTokenType(TokenType::LP)) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.isCurrentTokenType(TokenType::RP)) {
    const std::string strErr{"Error: missing missing condition, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto match{parseLogicalOr(currentScope, father)};
  if (tkStream_.isCurrentTokenType(TokenType::RP)) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right parenthesis, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.isCurrentTokenType(TokenType::LB)) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing left bracket, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.isCurrentTokenType(TokenType::RB)) {
    const std::string strErr{"Error: missing at least once case, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  std::vector<std::shared_ptr<NodeCaseStatement>> cases{};
  while (!tkStream_.isCurrentTokenType(TokenType::DEFAULT) &&
         !tkStream_.isCurrentTokenType(TokenType::RB)) {
    cases.push_back(parseCaseStatement(currentScope, father));
  }
  auto defaultCase{parseDefault(currentScope, father)};
  if (tkStream_.isCurrentTokenType(TokenType::RB)) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing right bracket, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  return ASTBuilder::createSwitch(match, cases, defaultCase);
  /*
  switch (true) {
    case 0: {

    }
    default: {

    }
  }
  */
}

std::shared_ptr<NodeCaseStatement>
TopDown::parseCaseStatement(std::shared_ptr<VariableTable> currentScope,
                            std::shared_ptr<Node> father) const {

  if (tkStream_.isCurrentTokenType(TokenType::CASE)) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing case, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto match{parseLogicalOr(currentScope, father)};

  if (tkStream_.isCurrentTokenType(TokenType::DOTDOT)) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing :, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto bodyScope{TBBuilder::createScope(currentScope)};
  auto body{parseBody(bodyScope, father)};

  return ASTBuilder::createCase(match, body);
}

std::shared_ptr<NodeCaseStatement>
TopDown::parseDefault(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const {

  if (tkStream_.isCurrentTokenType(TokenType::DEFAULT)) {
    tkStream_.eat();
  } else if (tkStream_.isCurrentTokenType(TokenType::RB)) {
    return nullptr;
  } else {
    const std::string strErr{"Error: missing default, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  if (tkStream_.isCurrentTokenType(TokenType::DOTDOT)) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error: missing :, found " +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }

  auto bodyScope{TBBuilder::createScope(currentScope)};
  auto body{parseBody(bodyScope, father)};

  return ASTBuilder::createCase(nullptr, body);
}

std::shared_ptr<NodeIfStatement>
TopDown::parseIfStatement(std::shared_ptr<VariableTable> currentScope,
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
    const std::string strErr{"Error: empty if condition, found " +
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
  auto ifScope{TBBuilder::createScope(currentScope)};
  auto ifBody{parseBody(ifScope, father)};
  std::shared_ptr<NodeStatementList> elseBody{nullptr};
  if (tkStream_.currentPos() < tkStream_.size() and
      tkStream_.current().type() == TokenType::ELSE) {
    tkStream_.eat();
    auto elseScope{TBBuilder::createScope(currentScope)};
    elseBody = parseBody(elseScope, father);
  }

  return ASTBuilder::createIf(condition, ifBody, elseBody);
}

std::shared_ptr<Node>
TopDown::parseTernary(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const {
  if (tkStream_.isCurrentTokenType(TokenType::LP) and
      tkStream_.isTokenAheadBeforeSemicolon(TokenType::TERNARY)) {
    tkStream_.eat();
    if (tkStream_.isCurrentTokenType(TokenType::RP)) {
      // ERROR
    }
    auto condition{parseLogicalOr(currentScope, father)};
    if (tkStream_.isCurrentTokenType(TokenType::RP)) {
      tkStream_.eat();
    } else {
      // error
    }
    if (tkStream_.isCurrentTokenType(TokenType::TERNARY)) {
      tkStream_.eat();
    } else {
      // error
    }
    auto first{parseLogicalOr(currentScope, father)};
    if (tkStream_.isCurrentTokenType(TokenType::DOTDOT)) {
      tkStream_.eat();
    } else {
      // error
    }
    auto second{parseLogicalOr(currentScope, father)};
    return ASTBuilder::createTernary(condition, first, second, father);
  }
  return parseLogicalOr(currentScope, father);
}

} // namespace nicole