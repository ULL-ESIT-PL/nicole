#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_CHAINED>, Error>
TopDown::parseChainedExpression() const noexcept {
  const Token id{*tkStream_.current()};
  auto base{Builder::createVarCall(id.raw())};
  if (!base || !*base) {
    return std::unexpected{base ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                : base.error()};
  }
  std::shared_ptr<AST> basePtr{*base};
  if (!tkStream_.eat()) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "Failed to consume token"}};
  }
  std::vector<std::shared_ptr<AST>> operations;
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() == TokenType::LP and
         tkStream_.current()->type() == TokenType::LC and
         tkStream_.current()->type() == TokenType::DOT) {
    switch (tkStream_.current()->type()) {
    case TokenType::LP: { // Function or method call
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume '('"}};
      }
      auto args = parseArguments(TokenType::RP);
      if (!args) {
        return std::unexpected{args.error()};
      }
      if (!operations.empty() &&
          operations.back()->type() == AST_TYPE::ATTR_ACCESS) {
        auto attrNode =
            std::static_pointer_cast<AST_ATTR_ACCESS>(operations.back());
        basePtr = *Builder::createMethodCall(attrNode->id(), *args);
        operations.pop_back(); // Quitamos el acceso al atributo porque ya es un
                               // método
      } else {
        const std::expected<std::shared_ptr<AST_FUNC_CALL>, Error> funCall{
            Builder::createFunCall(id.raw(), *args)};
        if (!funCall || !*funCall) {
          return std::unexpected{
              funCall ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                      : funCall.error()};
        }
        basePtr = *funCall;
      }
      break;
    }
    case TokenType::LB: { // Index access
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume '['"}};
      }
      auto index = parseOr();
      if (!index) {
        return std::unexpected{index.error()};
      }
      if (tkStream_.current()->type() != TokenType::RB || !tkStream_.eat()) {
        return std::unexpected{Error{ERROR_TYPE::SINTAX, "Missing ']'"}};
      }
      const std::expected<std::shared_ptr<AST_INDEX>, Error> indexAST{
          Builder::createIndex(*index)};
      if (!indexAST || !*indexAST) {
        return std::unexpected{
            indexAST ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                     : indexAST.error()};
      }
      operations.push_back(*indexAST);
      break;
    }
    case TokenType::DOT: { // Attribute access
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume '.'"}};
      }
      if (tkStream_.current()->type() != TokenType::ID) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Expected identifier after '.'"}};
      }
      const std::expected<std::shared_ptr<AST_ATTR_ACCESS>, Error> attr{
          Builder::createAttrAccess(tkStream_.current()->raw())};
      if (!attr || !*attr) {
        return std::unexpected{
            attr ? Error{ERROR_TYPE::NULL_NODE, "node is null"} : attr.error()};
      }
      operations.push_back(*attr);
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume identifier"}};
      }
      break;
    }
    default:
      return Builder::createChained(basePtr, operations);
    }
  }
  return Builder::createChained(basePtr, operations);
}

} // namespace nicole