#include "../../../../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdio>
#include <memory>

namespace nicole {

std::shared_ptr<NodeStructDeclaration>
TopDown::parseStructDeclaration(std::shared_ptr<VariableTable> currentScope,
                                std::shared_ptr<Node> father) const {
  tkStream_.eat();
  auto token{tkStream_.current()};
  std::shared_ptr<ParamsDeclaration> attributes{nullptr};
  //std::shared_ptr<UserType> idType{nullptr};
  std::string idTypeStr{""};
  if (token.type() == TokenType::ID) {
    idTypeStr = token.raw();
    //idType = std::make_shared<UserType>(idTypeStr);
    //typeTable_->addType(idType);
    tkStream_.eat();
    if (tkStream_.current().type() == TokenType::LB) {
      tkStream_.eat();
      auto structScope{TBBuilder::createScope(nullptr)};
      attributes = parseParams(structScope, father);
      //idType->setAttributes(attributes);
      if (tkStream_.current().type() == TokenType::RB) {
        tkStream_.eat();
      } else {
        const std::string strErr{"Error missing right } of " + idTypeStr +
                                 " at " + tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
    } else {
      const std::string strErr{"Error missing left { of " + idTypeStr + " at " +
                               tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
  }
  return ASTBuilder::createStructDecl(idTypeStr, attributes, typeTable_);
}

std::shared_ptr<NodeFunctionDeclaration>
TopDown::parseFunctionDeclaration(std::shared_ptr<VariableTable> currentScope,
                                  std::shared_ptr<Node> father) const {
  tkStream_.eat();
  auto token{tkStream_.current()};
  std::string id{};
  if (token.type() == TokenType::ID) {
    id = token.raw();
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing function id after def, found" +
                             tkStream_.current().raw() + " at " +
                             tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::LP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing left ( for function " + id +
                             " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  auto funScope{TBBuilder::createScope(nullptr)};
  std::shared_ptr<ParamsDeclaration> params{parseParams(funScope, father)};
  if (tkStream_.current().type() == TokenType::RP) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing right ) for function " + id +
                             " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  if (tkStream_.current().type() == TokenType::DOTDOT) {
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing \':\' after right ) for function " +
                             id + " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  token = tkStream_.current();
  std::shared_ptr<GenericType> returnType{nullptr};
  if (token.type() == TokenType::ID) {
    returnType = typeTable_->type(token.raw());
    tkStream_.eat();
  } else {
    const std::string strErr{"Error missing return type for the function " +
                             id + " at " + tkStream_.current().locInfo()};
    llvm::report_fatal_error(strErr.c_str());
  }
  std::shared_ptr<NodeStatementList> body{parseBody(funScope, father)};
  return ASTBuilder::createFunctDecl(id, params, returnType, body, funScope,
                                     typeTable_, functionTable_);
}

std::shared_ptr<NodeReturn>
TopDown::parseReturn(std::shared_ptr<VariableTable> &currentScope,
                     std::shared_ptr<Node> father) const {
  tkStream_.eat();
  if (tkStream_.isCurrentTokenType(TokenType::SEMICOLON)) {
    return ASTBuilder::createReturn(nullptr);
  }
  return ASTBuilder::createReturn(parseTernary(currentScope, father));
}

std::shared_ptr<Node>
TopDown::parseVarDeclaration(std::shared_ptr<VariableTable> currentScope,
                             std::shared_ptr<Node> father) const {
  Token token{tkStream_.current()};
  if (token.type() == TokenType::LET) {
    tkStream_.eat();
    token = tkStream_.current();
    if (token.type() == TokenType::ID) {
      const std::string id{token.raw()};
      tkStream_.eat();
      if (tkStream_.current().type() == TokenType::DOTDOT) {
        tkStream_.eat();
      } else {
        const std::string strErr{"Error missing \':\' after " + id + " at " +
                                 tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
      token = tkStream_.current();
      if (token.type() == TokenType::ID) {
        const std::string idTypeStr{token.raw()};
        //std::shared_ptr<GenericType> idType{typeTable_->type(idTypeStr)};
        tkStream_.eat();
        if (tkStream_.current().type() == TokenType::ASSIGNMENT) {
          tkStream_.eat();
          auto value{parseTernary(currentScope, father)};
          return ASTBuilder::createVarDecl(id, idTypeStr, value, currentScope,
                                           typeTable_);
        } else {
          const std::string strErr{"Error missing value of " + id + " at " +
                                   tkStream_.current().locInfo()};
          llvm::report_fatal_error(strErr.c_str());
        }

      } else {
        const std::string strErr{"Error missing type of " + id + " at " +
                                 tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
    }
  } else if (token.type() == TokenType::CONST) {
    tkStream_.eat();
    token = tkStream_.current();
    if (token.type() == TokenType::ID) {
      const std::string id{token.raw()};
      tkStream_.eat();
      if (tkStream_.current().type() == TokenType::DOTDOT) {
        tkStream_.eat();
      } else {
        const std::string strErr{"Error missing \':\' after " + id + " at " +
                                 tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
      token = tkStream_.current();
      if (token.type() == TokenType::ID) {
        const std::string idTypeStr{token.raw()};
        // std::shared_ptr<GenericType> idType{typeTable_->type(idTypeStr)};
        tkStream_.eat();
        if (tkStream_.current().type() == TokenType::ASSIGNMENT) {
          tkStream_.eat();
          auto value{parseTernary(currentScope, father)};
          return ASTBuilder::createConstDecl(id, idTypeStr, value, currentScope,
                                             typeTable_);
        } else {
          const std::string strErr{"Error missing value of " + id + " at " +
                                   tkStream_.current().locInfo()};
          llvm::report_fatal_error(strErr.c_str());
        }

      } else {
        const std::string strErr{"Error missing type of " + id + " at " +
                                 tkStream_.current().locInfo()};
        llvm::report_fatal_error(strErr.c_str());
      }
    }
  } else if (tkStream_.isCurrentTokenType(TokenType::AUTO)) {
    tkStream_.eat();
    token = tkStream_.current();
    std::string id{""};
    if (token.type() == TokenType::ID) {
      id = token.raw();
      tkStream_.eat();
    } else {
      const std::string strErr{"Error missing id at " +
                                   tkStream_.current().locInfo()};
      llvm::report_fatal_error(strErr.c_str());
    }
    if (tkStream_.current().type() == TokenType::ASSIGNMENT) {
          tkStream_.eat();
          auto value{parseTernary(currentScope, father)};
          return ASTBuilder::createAutoDecl(id, value, currentScope,
                                             typeTable_);
        } else {
          const std::string strErr{"Error missing value of " + id + " at " +
                                   tkStream_.current().locInfo()};
          llvm::report_fatal_error(strErr.c_str());
        }
  }
  
  return parseTernary(currentScope, father);
}

std::shared_ptr<NodeSelfReassignment>
TopDown::parseSelfAssignment(std::shared_ptr<VariableTable> currentScope,
                             std::shared_ptr<Node> father) const {
  const auto tk{tkStream_.current()};
  tkStream_.eat();
  const auto op{tkStream_.current()};
  tkStream_.eat();
  const auto expression{parseTernary(currentScope, father)};
  return ASTBuilder::createSelfRGT(tk.raw(), op.type(), expression,
                                   currentScope, typeTable_);
}

} // namespace nicole