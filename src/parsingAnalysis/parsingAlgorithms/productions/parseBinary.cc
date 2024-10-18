#include "../../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <memory>

namespace nicole {

std::shared_ptr<Node>
TopDown::parseLogicalOr(std::shared_ptr<VariableTable> currentScope,
                        std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseLogicalAnd(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         tkStream_.current().type() == TokenType::OR) {
    tkStream_.eat();
    auto right{parseLogicalAnd(currentScope, father)};
    left = ASTBuilder::createBinOp(left, TokenType::OR, right);
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseLogicalAnd(std::shared_ptr<VariableTable> currentScope,
                         std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseLogicalEqual(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         tkStream_.current().type() == TokenType::AND) {
    tkStream_.eat();
    auto right{parseLogicalEqual(currentScope, father)};
    left = ASTBuilder::createBinOp(left, TokenType::AND, right);
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseLogicalEqual(std::shared_ptr<VariableTable> currentScope,
                           std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseCompare(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current().type() == TokenType::EQUAL ||
          tkStream_.current().type() == TokenType::NOTEQUAL)) {
    const Token token{tkStream_.current()};
    tkStream_.eat();
    auto right{parseCompare(currentScope, father)};
    switch (token.type()) {
    case TokenType::EQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::EQUAL, right);
      break;
    case TokenType::NOTEQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::NOTEQUAL, right);
      break;
    default:
      const std::string strErr{"Cannot operate a binary expression with " +
                               tkStream_.current().raw() + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseCompare(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseAdd_Sub(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current().type() == TokenType::OPERATOR_SMALLER ||
          tkStream_.current().type() == TokenType::SMALLEREQUAL ||
          tkStream_.current().type() == TokenType::OPERATOR_GREATER ||
          tkStream_.current().type() == TokenType::BIGGEREQUAL)) {
    const Token token{tkStream_.current()};
    tkStream_.eat();
    auto right{parseAdd_Sub(currentScope, father)};
    switch (token.type()) {
    case TokenType::OPERATOR_SMALLER:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_SMALLER, right);
      break;
    case TokenType::SMALLEREQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::SMALLEREQUAL, right);
      break;
    case TokenType::OPERATOR_GREATER:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_GREATER, right);
      break;
    case TokenType::BIGGEREQUAL:
      left = ASTBuilder::createBinOp(left, TokenType::BIGGEREQUAL, right);
      break;
    default:
      const std::string strErr{"Cannot operate a binary expression with " +
                               tkStream_.current().raw() + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseAdd_Sub(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseMult_Div(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current().type() == TokenType::OPERATOR_ADD ||
          tkStream_.current().type() == TokenType::OPERATOR_SUB)) {
    const Token token{tkStream_.current()};
    tkStream_.eat();
    auto right{parseMult_Div(currentScope, father)};
    switch (token.type()) {
    case TokenType::OPERATOR_ADD:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_ADD, right);
      break;
    case TokenType::OPERATOR_SUB:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_SUB, right);
      break;
    default:
      const std::string strErr{"Cannot operate a binary expression with " +
                               tkStream_.current().raw() + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }

  return left;
}

std::shared_ptr<Node>
TopDown::parseMult_Div(std::shared_ptr<VariableTable> currentScope,
                       std::shared_ptr<Node> father) const {
  // equivalent to the first e + e in Jison recursively goes down
  auto left{parseFactor(currentScope, father)};

  while (tkStream_.currentPos() < tkStream_.size() &&
         (tkStream_.current().type() == TokenType::OPERATOR_MULT ||
          tkStream_.current().type() == TokenType::OPERATOR_DIV ||
          tkStream_.current().type() == TokenType::OPERATOR_MODULE)) {
    const Token token{tkStream_.current()};
    tkStream_.eat();
    auto right{parseFactor(currentScope, father)};
    switch (token.type()) {
    case TokenType::OPERATOR_MULT:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_MULT, right);
      break;
    case TokenType::OPERATOR_DIV:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_DIV, right);
      break;
    case TokenType::OPERATOR_MODULE:
      left = ASTBuilder::createBinOp(left, TokenType::OPERATOR_MODULE, right);
      break;
    default:
      const std::string strErr{"Cannot operate a binary expression with " +
                               tkStream_.current().raw() + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }

  return left;
}

}