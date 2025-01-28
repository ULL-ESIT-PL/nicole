#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_CHAINED>, Error>
TopDown::parseChainedExpression() const noexcept {
  std::shared_ptr<AST> basePtr{nullptr};
  std::vector<std::shared_ptr<AST>> operations{};
  const Token id{*tkStream_.current()};

  // Verifica si es un identificador
  if (tkStream_.current()->type() != TokenType::ID) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "Expected identifier at " + id.locInfo()}};
  }

  if (!tkStream_.eat()) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "Failed to consume token"}};
  }

  // Si no hay operadores adicionales (ni `.` ni `(` ni `[`), es solo una
  // variable
  if (tkStream_.current()->type() != TokenType::DOT &&
      tkStream_.current()->type() != TokenType::LP &&
      tkStream_.current()->type() != TokenType::LB) {
    auto varCall = Builder::createVarCall(id.raw());
    if (!varCall) {
      return std::unexpected{
          Error{ERROR_TYPE::NULL_NODE, "Failed to create var call"}};
    }
    return Builder::createChained(*varCall, operations);
  }

  // Si hay `(`, es una función en vez de una variable
  if (tkStream_.current()->type() == TokenType::LP) {
    if (!tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "Failed to consume '('"}};
    }

    auto args = parseArguments();
    if (!args) {
      return std::unexpected{args.error()};
    }

    if (tkStream_.current()->type() != TokenType::RP || !tkStream_.eat()) {
      return std::unexpected{Error{ERROR_TYPE::SINTAX, "Missing ')'"}};
    }

    auto funcCall = Builder::createFunCall(id.raw(), *args);
    if (!funcCall) {
      return std::unexpected{
          Error{ERROR_TYPE::NULL_NODE, "Failed to create func call"}};
    }
    basePtr = *funcCall;
  } else {
    // Es una llamada a variable
    auto varCall = Builder::createVarCall(id.raw());
    if (!varCall) {
      return std::unexpected{
          Error{ERROR_TYPE::NULL_NODE, "Failed to create var call"}};
    }
    basePtr = *varCall;
  }

  // Manejo de expresiones encadenadas (atributos, métodos e índices)
  while (tkStream_.currentPos() < tkStream_.size()) {
    switch (tkStream_.current()->type()) {
    case TokenType::DOT: { // Acceso a atributos o métodos
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume '.'"}};
      }
      if (tkStream_.current()->type() != TokenType::ID) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Expected identifier after '.'"}};
      }

      Token attrToken = *tkStream_.current();
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume attribute name"}};
      }

      // Verificamos si es una llamada a método (método() en vez de atributo)
      if (tkStream_.current()->type() == TokenType::LP) {
        if (!tkStream_.eat()) {
          return std::unexpected{
              Error{ERROR_TYPE::SINTAX, "Failed to consume '('"}};
        }
        auto args = parseArguments();
        if (!args) {
          return std::unexpected{args.error()};
        }
        if (tkStream_.current()->type() != TokenType::RP || !tkStream_.eat()) {
          return std::unexpected{Error{ERROR_TYPE::SINTAX, "Missing ')'"}};
        }

        auto methodCall = Builder::createMethodCall(attrToken.raw(), *args);
        if (!methodCall) {
          return std::unexpected{
              Error{ERROR_TYPE::NULL_NODE, "Failed to create method call"}};
        }
        operations.push_back(*methodCall);
      } else {
        auto attrAccess = Builder::createAttrAccess(attrToken.raw());
        if (!attrAccess) {
          return std::unexpected{
              Error{ERROR_TYPE::NULL_NODE, "Failed to create attr access"}};
        }
        operations.push_back(*attrAccess);
      }
      break;
    }

    case TokenType::LC: { // Acceso a índices (`a[index]`)
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume '['"}};
      }
      auto index = parseOr();
      if (!index) {
        return std::unexpected{index.error()};
      }
      if (tkStream_.current()->type() != TokenType::RC || !tkStream_.eat()) {
        return std::unexpected{Error{ERROR_TYPE::SINTAX, "Missing ']'"}};
      }
      auto indexAccess = Builder::createIndex(*index);
      if (!indexAccess) {
        return std::unexpected{
            Error{ERROR_TYPE::NULL_NODE, "Failed to create index access"}};
      }
      operations.push_back(*indexAccess);
      break;
    }

    default:
      return Builder::createChained(basePtr, operations);
    }
  }
  return Builder::createChained(basePtr, operations);
}

} // namespace nicole