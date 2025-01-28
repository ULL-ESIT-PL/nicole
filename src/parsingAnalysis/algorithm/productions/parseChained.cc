#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_CHAINED>, Error>
TopDown::parseChainedExpression() const noexcept {
  // 1. Verificar que el primer token sea ID
  if (tkStream_.current()->type() != TokenType::ID) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "Expected identifier at " + tkStream_.current()->locInfo()}};
  }

  const Token baseToken{*tkStream_.current()};
  if (!tkStream_.eat()) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX,
              "Failed to consume identifier at " + baseToken.locInfo()}};
  }

  // 2. Determinar si es un simple varCall o una funcCall
  std::shared_ptr<AST> basePtr = nullptr;
  if (tkStream_.current()->type() == TokenType::LP) {
    // Llamada a función
    if (!tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "Failed to consume '(' at " + tkStream_.current()->locInfo()}};
    }

    // Parsear argumentos hasta ')'
    std::vector<std::shared_ptr<AST>> args;
    while (tkStream_.currentPos() < tkStream_.size() &&
           tkStream_.current()->type() != TokenType::RP) {
      auto expr = parseOr(); // parseOr() ya implementado
      if (!expr || !*expr) {
        return std::unexpected{
            expr ? Error{ERROR_TYPE::NULL_NODE, "node is null"} : expr.error()};
      }
      args.push_back(*expr);
      if (tkStream_.current()->type() == TokenType::COMMA) {
        if (!tkStream_.eat()) {
          return std::unexpected{
              Error{ERROR_TYPE::SINTAX, "Failed to consume ',' at " +
                                            tkStream_.current()->locInfo()}};
        }
        continue;
      } else if (tkStream_.current()->type() != TokenType::RP) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX,
                  "Missing comma or ')' at " + tkStream_.current()->locInfo()}};
      }
      break;
    }

    // Consumir ')'
    if (tkStream_.current()->type() != TokenType::RP || !tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "Missing ')' at " + tkStream_.current()->locInfo()}};
    }

    // Crear nodo de función
    auto funcCall = Builder::createFunCall(baseToken.raw(), args);
    if (!funcCall || !*funcCall) {
      return std::unexpected{
          funcCall ? Error{ERROR_TYPE::NULL_NODE, "Failed to create func call"}
                   : funcCall.error()};
    }
    basePtr = *funcCall;
  } else {
    // Variable normal
    auto varCall = Builder::createVarCall(baseToken.raw());
    if (!varCall || !*varCall) {
      return std::unexpected{
          varCall ? Error{ERROR_TYPE::NULL_NODE, "Failed to create var call"}
                  : varCall.error()};
    }
    basePtr = *varCall;
  }

  // 3. Operaciones encadenadas: [index], .atributo, .metodo(...)
  std::vector<std::shared_ptr<AST>> operations;
  while (tkStream_.currentPos() < tkStream_.size() and
         (tkStream_.current()->type() == TokenType::LC or
          tkStream_.current()->type() == TokenType::DOT)) {
    TokenType ttype = tkStream_.current()->type();
    switch (ttype) {
    // Acceso a índice -> base[index]
    case TokenType::LC: {
      // Consumir '['
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume '[' at " +
                                          tkStream_.current()->locInfo()}};
      }
      // parseOr() para la expresión de índice
      auto indexExpr = parseOr();
      if (!indexExpr || !*indexExpr) {
        return std::unexpected{
            indexExpr ? Error{ERROR_TYPE::NULL_NODE, "index is null"}
                      : indexExpr.error()};
      }
      // Consumir ']'
      if (tkStream_.current()->type() != TokenType::RC || !tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX,
                  "Missing ']' at " + tkStream_.current()->locInfo()}};
      }
      // Crear nodo de índice
      auto indexNode = Builder::createIndex(*indexExpr);
      if (!indexNode || !*indexNode) {
        return std::unexpected{indexNode ? Error{ERROR_TYPE::NULL_NODE,
                                                 "Failed to create index node"}
                                         : indexNode.error()};
      }
      operations.push_back(*indexNode);
      break;
    }

    // Punto -> base.atributo o base.metodo(...)
    case TokenType::DOT: {
      // Consumir '.'
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume '.' at " +
                                          tkStream_.current()->locInfo()}};
      }
      if (tkStream_.current()->type() != TokenType::ID) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Expected identifier after '.' at " +
                                          tkStream_.current()->locInfo()}};
      }

      // Leer el nombre del atributo/método
      Token attrToken{*tkStream_.current()};
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume attribute id at " +
                                          tkStream_.current()->locInfo()}};
      }

      // Comprobar si es método -> un '(' a continuación
      if (tkStream_.current()->type() == TokenType::LP) {
        // Consumir '('
        if (!tkStream_.eat()) {
          return std::unexpected{
              Error{ERROR_TYPE::SINTAX, "Failed to consume '(' at " +
                                            tkStream_.current()->locInfo()}};
        }

        // Parsear parámetros
        std::vector<std::shared_ptr<AST>> args;
        while (tkStream_.currentPos() < tkStream_.size() &&
               tkStream_.current()->type() != TokenType::RP) {
          auto param = parseOr();
          if (!param || !*param) {
            return std::unexpected{
                param ? Error{ERROR_TYPE::NULL_NODE, "param is null"}
                      : param.error()};
          }
          args.push_back(*param);
          if (tkStream_.current()->type() == TokenType::COMMA) {
            if (!tkStream_.eat()) {
              return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                           "Failed to consume ',' at " +
                                               tkStream_.current()->locInfo()}};
            }
            continue;
          } else if (tkStream_.current()->type() != TokenType::RP) {
            return std::unexpected{
                Error{ERROR_TYPE::SINTAX, "Missing comma or ')' at " +
                                              tkStream_.current()->locInfo()}};
          }
          break;
        }

        // Consumir ')'
        if (tkStream_.current()->type() != TokenType::RP || !tkStream_.eat()) {
          return std::unexpected{
              Error{ERROR_TYPE::SINTAX,
                    "Missing ')' at " + tkStream_.current()->locInfo()}};
        }

        auto methodNode = Builder::createMethodCall(attrToken.raw(), args);
        if (!methodNode || !*methodNode) {
          return std::unexpected{
              methodNode
                  ? Error{ERROR_TYPE::NULL_NODE, "Failed to create method call"}
                  : methodNode.error()};
        }
        operations.push_back(*methodNode);
      } else {
        // Es un atributo
        auto attrNode = Builder::createAttrAccess(attrToken.raw());
        if (!attrNode || !*attrNode) {
          return std::unexpected{
              attrNode ? Error{ERROR_TYPE::NULL_NODE,
                               "Failed to create attribute access"}
                       : attrNode.error()};
        }
        operations.push_back(*attrNode);
      }
      break;
    }

    default:
      // Si ya no es ni '[', ni '.', ni '(', terminamos
      return Builder::createChained(basePtr, operations);
    }
  }

  // Si se acaban los tokens, retornamos la cadena
  return Builder::createChained(basePtr, operations);
}

} // namespace nicole