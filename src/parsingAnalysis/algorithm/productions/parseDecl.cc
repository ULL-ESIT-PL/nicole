#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseVarDecl(const bool insideFor) const noexcept {
  const Token token{*tkStream_.current()};
  std::string varName;
  std::string varType;
  std::shared_ptr<AST> valueExpr;

  // Función auxiliar para manejar la parte común de las declaraciones
  auto parseCommon = [&]() -> std::expected<void, Error> {
    // Verificar si el siguiente token es un identificador (nombre de la
    // variable)
    if (tkStream_.current()->type() != TokenType::ID) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "Expected variable name at " + tkStream_.current()->locInfo()}};
    }
    varName = tkStream_.current()->raw();
    if (!tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "Failed to consume variable name at " +
                                        tkStream_.current()->locInfo()}};
    }

    // Verificar si hay una especificación de tipo
    if (tkStream_.current()->type() == TokenType::DOTDOT) {
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume ':' at " +
                                          tkStream_.current()->locInfo()}};
      }
      if (tkStream_.current()->type() != TokenType::ID) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX,
                  "Expected type name at " + tkStream_.current()->locInfo()}};
      }
      varType = tkStream_.current()->raw();
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume type name at " +
                                          tkStream_.current()->locInfo()}};
      }
    }

    // Verificar si hay un operador de asignación '='
    if (tkStream_.current()->type() == TokenType::ASSIGNMENT) {
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume '=' at " +
                                          tkStream_.current()->locInfo()}};
      }
      // Parsear la expresión de valor
      auto expr = parseTernary();
      if (!expr || !*expr) {
        return std::unexpected{
            expr ? Error{ERROR_TYPE::NULL_NODE, "Expression is null"}
                 : expr.error()};
      }
      valueExpr = *expr;
    } else {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "Expected '=' after variable declaration at " +
                                  tkStream_.current()->locInfo()}};
    }

    // Verificar el punto y coma al final de la declaración
    if (!insideFor) {
      if (tkStream_.current()->type() != TokenType::SEMICOLON) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX,
                  "Expected ';' at the end of variable declaration at " +
                      tkStream_.current()->locInfo()}};
      }
      if (!tkStream_.eat()) {
        return std::unexpected{
            Error{ERROR_TYPE::SINTAX, "Failed to consume ';' at " +
                                          tkStream_.current()->locInfo()}};
      }
    }
    return {};
  };

  switch (token.type()) {
  case TokenType::CONST: { // const variable: type = expression;
    if (!tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "Failed to consume 'const' at " +
                                        tkStream_.current()->locInfo()}};
    }
    auto result = parseCommon();
    if (!result) {
      return std::unexpected{result.error()};
    }
    if (varType.empty()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "Const variable must have a type specified at " +
                                  tkStream_.current()->locInfo()}};
    }
    return Builder::createConstDecl(varName, varType, valueExpr);
  }
  case TokenType::LET: { // let variable: type = expression;
    if (!tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "Failed to consume 'let' at " +
                                        tkStream_.current()->locInfo()}};
    }
    auto result = parseCommon();
    if (!result) {
      return std::unexpected{result.error()};
    }
    if (varType.empty()) {
      return std::unexpected{Error{
          ERROR_TYPE::SINTAX, "Let variable must have a type specified at " +
                                  tkStream_.current()->locInfo()}};
    }
    return Builder::createLetDecl(varName, varType, valueExpr);
  }
  case TokenType::AUTO: { // auto variable = expression;
    if (!tkStream_.eat()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX, "Failed to consume 'auto' at " +
                                        tkStream_.current()->locInfo()}};
    }
    auto result = parseCommon();
    if (!result) {
      return std::unexpected{result.error()};
    }
    if (!varType.empty()) {
      return std::unexpected{
          Error{ERROR_TYPE::SINTAX,
                "Auto variable should not have a type specified at " +
                    tkStream_.current()->locInfo()}};
    }
    return Builder::createAutoDecl(varName, valueExpr);
  }
  default: {
    return parseTernary();
  }
  }
}

const std::expected<std::shared_ptr<AST_DELETE>, Error>
TopDown::parseDelete() const noexcept {
  if (!tkStream_.eat()) {
    return std::unexpected{
        Error{ERROR_TYPE::SINTAX, "Failed to consume 'delete' at " +
                                      tkStream_.current()->locInfo()}};
  }
  const std::expected<std::shared_ptr<AST>, Error> value{parseOr()};
  if (!value || !*value) {
    return std::unexpected{value ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : value.error()};
  }
  if (tkStream_.current()->type() != TokenType::SEMICOLON) {
    return std::unexpected{Error{
        ERROR_TYPE::SINTAX, "Expected ';' at the end of delete expression at " +
                                tkStream_.current()->locInfo()}};
  }
  return Builder::createDelete(*value);
}

} // namespace nicole
