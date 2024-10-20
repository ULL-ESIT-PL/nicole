#include "../../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <memory>

namespace nicole {

std::shared_ptr<Node>
TopDown::parseFactor(std::shared_ptr<VariableTable> currentScope,
                     std::shared_ptr<Node> father) const {
  switch (tkStream_.current().type()) {
  case TokenType::NUMBER_INT: {
    const int value{std::stoi(tkStream_.current().raw())};
    tkStream_.eat();
    return ASTBuilder::createInt(value, father);
  }
  case TokenType::NUMBER_DOUBLE: {
    const double value{std::stod(tkStream_.current().raw())};
    tkStream_.eat();
    return ASTBuilder::createDouble(value, father);
  }
  case TokenType::NUMBER_FLOAT: {
    const float value{std::stof(tkStream_.current().raw().substr(1))};
    tkStream_.eat();
    return ASTBuilder::createFloat(value, father);
  }
  case TokenType::STRING: {
    const std::string value{tkStream_.current().raw()};
    tkStream_.eat();
    return ASTBuilder::createString(value, father);
  }
  case TokenType::CHAR: {
    const std::string value{tkStream_.current().raw()};
    tkStream_.eat();
    return ASTBuilder::createChar(value, father);
  }
  case TokenType::TRUE: {
    tkStream_.eat();
    return ASTBuilder::createBool(true);
  }
  case TokenType::FALSE: {
    tkStream_.eat();
    return ASTBuilder::createBool(false);
  }
  case TokenType::OPERATOR_NOT: {
    tkStream_.eat();
    return ASTBuilder::createUnaryOp(TokenType::OPERATOR_NOT,
                                     parseLogicalOr(currentScope, father));
  }
  case TokenType::OPERATOR_SUB: {
    tkStream_.eat();
    return ASTBuilder::createUnaryOp(TokenType::OPERATOR_SUB,
                                     parseLogicalOr(currentScope, father));
  }
  case TokenType::INCREMENT: {
    tkStream_.eat();
    return ASTBuilder::createIncrement(TokenType::INCREMENT,
                                       parseLogicalOr(currentScope, father));
  }
  case TokenType::DECREMENT: {
    tkStream_.eat();
    return ASTBuilder::createIncrement(TokenType::DECREMENT,
                                       parseLogicalOr(currentScope, father));
  }
  case TokenType::ID: {
    const std::string id{tkStream_.current().raw()};
    tkStream_.eat();
    if (tkStream_.current().type() == TokenType::ASSIGNMENT) {
      tkStream_.eat();
      auto expression{parseLogicalOr(currentScope, father)};
      return ASTBuilder::createVarRGT(id, expression, currentScope, typeTable_);
    } else if (tkStream_.current().type() == TokenType::LB) {
      tkStream_.eat();
      auto attributes{parseParamsCall(currentScope, father)};
      if (tkStream_.current().type() == TokenType::RB) {
        tkStream_.eat();
        return ASTBuilder::createStructConstr(id, attributes, typeTable_);
      }
    } else if (tkStream_.current().type() == TokenType::LP) {
      return parseFunctionCall(id, currentScope, father);
    }
    return ASTBuilder::createVarCall(id, currentScope);
  }
  case TokenType::STOP: {
    tkStream_.eat();
    return ASTBuilder::createStop(father);
  }
  case TokenType::PASS: {
    tkStream_.eat();
    return ASTBuilder::createPass(father);
  }
  case TokenType::LP: {
    tkStream_.eat();
    auto expression{parseLogicalOr(currentScope, father)};
    if (tkStream_.current().type() == TokenType::RP) {
      tkStream_.eat();
    } else {
      const std::string strErr{"Error: missing right parenthesis, found " +
                               tkStream_.current().raw() + " at" +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
    return expression;
  }
  default:
    const std::string strErr{"Error: unknown token found " +
                             tkStream_.current().raw() + " at" +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
}

std::shared_ptr<NodeFunctionCall>
TopDown::parseFunctionCall(const std::string &id,
                           std::shared_ptr<VariableTable> currentScope,
                           std::shared_ptr<Node> father) const {
  tkStream_.eat();
  std::vector<std::shared_ptr<Node>> params{};
  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
    return ASTBuilder::createFunctCall(id, params, currentScope,
                                       functionTable_);
  }
  params = parseParamsCall(currentScope, father);
  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing right ) for function " + id +
                             " at" + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  return ASTBuilder::createFunctCall(id, params, currentScope, functionTable_);
}

} // namespace nicole