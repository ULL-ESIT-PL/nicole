#include "../../inc/parsingAnalysis/builder.h"

namespace nicole {

std::expected<std::shared_ptr<AST_BOOL>, Error>
Builder::createBool(const bool value) noexcept {
  return std::make_shared<AST_BOOL>(value);
}

std::expected<std::shared_ptr<AST_CHAR>, Error>
Builder::createChar(const std::string &value) noexcept {
  const std::string strNoQuotes{value.substr(1, value.size() - 2)};
  char result;
  if (strNoQuotes.size() == 1) {
    result = strNoQuotes[0];
  }
  // Si hay un carácter de escape, comprobar cuál es
  if (strNoQuotes[0] == '\\' && strNoQuotes.size() == 2) {
    switch (strNoQuotes[1]) {
    case 'n':
      result = '\n';
    case 't':
      result = '\t';
    case '\\':
      result = '\\';
    case '\'':
      result = '\'';
    case '\"':
      result = '\"';
    default:
      result = strNoQuotes[1];
    }
  }
  return std::make_shared<AST_CHAR>(result);
}

std::expected<std::shared_ptr<AST_DOUBLE>, Error>
Builder::createDouble(const double value) noexcept {
  return std::make_shared<AST_DOUBLE>(value);
}

std::expected<std::shared_ptr<AST_FLOAT>, Error>
Builder::createFloat(const float value) noexcept {
  return std::make_shared<AST_FLOAT>(value);
}

std::expected<std::shared_ptr<AST_INT>, Error>
Builder::createInt(const int value) noexcept {
  return std::make_shared<AST_INT>(value);
}

std::expected<std::shared_ptr<AST_NULL>, Error> Builder::createNull() noexcept {
  return std::make_shared<AST_NULL>();
}

std::expected<std::shared_ptr<AST_STRING>, Error>
Builder::createString(const std::string value) noexcept {
  std::string result;
  const std::string strNoQuotes{value.substr(1, value.size() - 2)};
  for (size_t i = 0; i < value.length(); ++i) {
    if (strNoQuotes[i] == '\\' && i + 1 < value.length()) {
      switch (strNoQuotes[i + 1]) {
      case 'n':
        result += '\n';
        break;
      case 't':
        result += '\t';
        break;
      case '\\':
        result += '\\';
        break;
      case '\"':
        result += '\"';
        break;
      default:
        result += strNoQuotes[i + 1];
        break;
      }
      i++;
    } else {
      result += strNoQuotes[i]; // Agregar el carácter actual
    }
  }
  return std::make_shared<AST_STRING>(result);
}

std::expected<std::shared_ptr<AST_VECTOR>, Error>
Builder::createVector(const std::vector<std::shared_ptr<AST>> values) noexcept {
  const auto astVector{std::make_shared<AST_VECTOR>(values)};
  const std::vector<std::shared_ptr<AST>> &expressions{astVector->values()};
  for (const auto &expression : expressions) {
    if (expression) {
      expression->setFather(astVector);
    }
  }
  return astVector;
}

std::expected<std::shared_ptr<AST_INDEX>, Error>
Builder::createIndex(const std::shared_ptr<AST> value) noexcept {
  const auto astIndex{std::make_shared<AST_INDEX>(value)};
  if (value) {
    value->setFather(astIndex);
  }
  return astIndex;
}

std::expected<std::shared_ptr<AST_DELETE>, Error>
Builder::createDelete(const std::shared_ptr<AST> &value) noexcept {
  const auto astDelete{std::make_shared<AST_DELETE>(value)};
  if (value) {
    value->setFather(astDelete);
  }
  return astDelete;
}

std::expected<std::shared_ptr<AST_NEW>, Error>
Builder::createNew(const std::shared_ptr<AST> &value) noexcept {
  const auto astNew{std::make_shared<AST_NEW>(value)};
  if (value) {
    value->setFather(astNew);
  }
  return astNew;
}

std::expected<std::shared_ptr<AST_DEREF>, Error>
Builder::createDeref(const std::shared_ptr<AST> &value) noexcept {
  const auto astDeref{std::make_shared<AST_DEREF>(value)};
  if (value) {
    value->setFather(astDeref);
  }
  return astDeref;
}

std::expected<std::shared_ptr<AST_PTR>, Error>
Builder::createPtr(const std::string &id, const std::string &type,
                   const std::shared_ptr<AST> &value) noexcept {
  const auto astPtr{std::make_shared<AST_PTR>(id, type, value)};
  if (value) {
    value->setFather(astPtr);
  }
  return astPtr;
}

std::expected<std::shared_ptr<AST_BINARY>, Error>
Builder::createBinary(const Token &op, const std::shared_ptr<AST> &left,
                      const std::shared_ptr<AST> &right) noexcept {
  const auto astBinary{std::make_shared<AST_BINARY>(op, left, right)};
  if (left) {
    left->setFather(astBinary);
  }
  if (right) {
    right->setFather(astBinary);
  }
  return astBinary;
}

std::expected<std::shared_ptr<AST_UNARY>, Error>
Builder::createUnary(const Token &op,
                     const std::shared_ptr<AST> &value) noexcept {
  const auto astUnary{std::make_shared<AST_UNARY>(op, value)};
  if (value) {
    value->setFather(astUnary);
  }
  return astUnary;
}

std::expected<std::shared_ptr<AST_ASSIGNMENT>, Error>
Builder::createAssignment(const Token &op, const std::shared_ptr<AST> &left,
                          const std::shared_ptr<AST> &value) noexcept {
  const auto astSelfAssignment{
      std::make_shared<AST_ASSIGNMENT>(op, left, value)};
  if (left) {
    left->setFather(astSelfAssignment);
  }
  if (value) {
    value->setFather(astSelfAssignment);
  }
  return astSelfAssignment;
}

std::expected<std::shared_ptr<AST_PRINT>, Error>
Builder::createPrint(const std::vector<std::shared_ptr<AST>> &values) noexcept {
  const auto astPrint{std::make_shared<AST_PRINT>(values)};
  const std::vector<std::shared_ptr<AST>> &values__{astPrint->values()};
  for (const auto &values_ : values__) {
    if (values_) {
      values_->setFather(astPrint);
    }
  }
  return astPrint;
}

std::expected<std::shared_ptr<AST_IMPORT>, Error>
Builder::createImport(const std::filesystem::path &path) noexcept {
  const auto astImport{std::make_shared<AST_IMPORT>(path)};
  return astImport;
}

std::expected<std::shared_ptr<AST_STATEMENT>, Error>
Builder::createStatement(const std::shared_ptr<AST> &expression) noexcept {
  const auto astStatement{std::make_shared<AST_STATEMENT>(expression)};
  if (expression) {
    expression->setFather(astStatement);
  }
  return astStatement;
}

std::expected<std::shared_ptr<AST_BODY>, Error> Builder::createBody(
    const std::vector<std::shared_ptr<AST_STATEMENT>> &body) noexcept {
  const auto astBody{std::make_shared<AST_BODY>(body)};
  const std::vector<std::shared_ptr<AST_STATEMENT>> &statements{
      astBody->body()};
  for (const auto &statement : statements) {
    if (statement) {
      statement->setFather(astBody);
    }
  }
  return astBody;
}

std::expected<std::shared_ptr<AST_WHILE>, Error>
Builder::createWhile(const std::shared_ptr<AST_CONDITION> &condition,
                     const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astWhile{std::make_shared<AST_WHILE>(condition, body)};
  if (condition) {
    condition->setFather(astWhile);
  }
  if (body) {
    body->setFather(astWhile);
  }
  return astWhile;
}

std::expected<std::shared_ptr<AST_FOR>, Error>
Builder::createFor(const std::vector<std::shared_ptr<AST>> &init,
                   const std::shared_ptr<AST_CONDITION> &condition,
                   const std::vector<std::shared_ptr<AST>> &update,
                   const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astFor{std::make_shared<AST_FOR>(init, condition, update, body)};
  const std::vector<std::shared_ptr<AST>> &init__{astFor->init()};
  for (const auto &init_ : init__) {
    if (init_) {
      astFor->setFather(init_);
    }
  }
  if (condition) {
    condition->setFather(astFor);
  }
  const std::vector<std::shared_ptr<AST>> &update__{astFor->update()};
  for (const auto &update_ : update__) {
    if (update_) {
      astFor->setFather(update_);
    }
  }
  if (body) {
    body->setFather(astFor);
  }
  return astFor;
}

std::expected<std::shared_ptr<AST_DO_WHILE>, Error> Builder::createDoWhile(
    const std::shared_ptr<AST_BODY> &body,
    const std::shared_ptr<AST_CONDITION> &condition) noexcept {
  const auto astDoWhile{std::make_shared<AST_DO_WHILE>(body, condition)};
  if (condition) {
    condition->setFather(astDoWhile);
  }
  if (body) {
    body->setFather(astDoWhile);
  }
  return astDoWhile;
}

std::expected<std::shared_ptr<AST_PASS>, Error>
Builder::createPass(const std::shared_ptr<AST> &fatherLoop) noexcept {
  const auto astPass{std::make_shared<AST_PASS>(fatherLoop)};
  return astPass;
}

std::expected<std::shared_ptr<AST_STOP>, Error>
Builder::createStop(const std::shared_ptr<AST> &fatherLoop) noexcept {
  const auto astStop{std::make_shared<AST_STOP>(fatherLoop)};
  return astStop;
}

std::expected<std::shared_ptr<AST_IF>, Error>
Builder::createIf(const std::shared_ptr<AST_CONDITION> &condition,
                  const std::shared_ptr<AST_BODY> &body,
                  const std::vector<std::shared_ptr<AST_ELSE_IF>> &elseIf,
                  const std::shared_ptr<AST_BODY> &elseBody) noexcept {
  const auto astIf{std::make_shared<AST_IF>(condition, body, elseIf, elseBody)};
  if (condition) {
    condition->setFather(astIf);
  }
  if (body) {
    body->setFather(astIf);
  }
  const std::vector<std::shared_ptr<AST_ELSE_IF>> &elseIfs{astIf->elseIf()};
  for (const auto &elseIf_ : elseIfs) {
    if (elseIf_) {
      elseIf_->setFather(astIf);
    }
  }
  if (elseBody) {
    elseBody->setFather(astIf);
  }
  return astIf;
}

std::expected<std::shared_ptr<AST_ELSE_IF>, Error>
Builder::createElseIf(const std::shared_ptr<AST_CONDITION> &condition,
                      const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astIf{std::make_shared<AST_ELSE_IF>(condition, body)};
  if (condition) {
    condition->setFather(astIf);
  }
  if (body) {
    body->setFather(astIf);
  }
  return astIf;
}

std::expected<std::shared_ptr<AST_SWITCH>, Error>
Builder::createSwitch(const std::shared_ptr<AST_CONDITION> &condition,
                      const std::vector<std::shared_ptr<AST_CASE>> &cases,
                      const std::shared_ptr<AST_DEFAULT> &default_) noexcept {
  const auto astSwitch{
      std::make_shared<AST_SWITCH>(condition, cases, default_)};
  if (condition) {
    condition->setFather(astSwitch);
  }
  const std::vector<std::shared_ptr<AST_CASE>> &cases_{astSwitch->cases()};
  for (const auto &statement : cases_) {
    if (statement) {
      statement->setFather(astSwitch);
    }
  }
  if (default_) {
    default_->setFather(astSwitch);
  }
  return astSwitch;
}

std::expected<std::shared_ptr<AST_CASE>, Error>
Builder::createCase(const std::shared_ptr<AST> &match,
                    const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astCase{std::make_shared<AST_CASE>(match, body)};
  if (match) {
    match->setFather(astCase);
  }
  if (body) {
    body->setFather(astCase);
  }
  return astCase;
}

std::expected<std::shared_ptr<AST_DEFAULT>, Error>
Builder::createDefault(const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astDefault{std::make_shared<AST_DEFAULT>(body)};
  if (body) {
    body->setFather(astDefault);
  }
  return astDefault;
}

std::expected<std::shared_ptr<AST_TERNARY>, Error>
Builder::createTernary(const std::shared_ptr<AST_CONDITION> &condition,
                       const std::shared_ptr<AST> &first,
                       const std::shared_ptr<AST> &second) noexcept {
  const auto astTernanry{
      std::make_shared<AST_TERNARY>(condition, first, second)};
  if (condition) {
    condition->setFather(astTernanry);
  }
  if (first) {
    first->setFather(astTernanry);
  }
  if (second) {
    second->setFather(astTernanry);
  }
  return astTernanry;
}

std::expected<std::shared_ptr<AST_CONDITION>, Error>
Builder::createCondition(const std::shared_ptr<AST> &expression) noexcept {
  const auto astCondition{std::make_shared<AST_CONDITION>(expression)};
  if (expression) {
    expression->setFather(astCondition);
  }
  return astCondition;
}

std::expected<std::shared_ptr<AST_FUNC_CALL>, Error> Builder::createFunCall(
    const std::string &id,
    const std::vector<std::shared_ptr<AST>> &parameters) noexcept {
  const auto astFuncCall{std::make_shared<AST_FUNC_CALL>(id, parameters)};
  const std::vector<std::shared_ptr<AST>> &parameters__{
      astFuncCall->parameters()};
  for (const auto &parameters_ : parameters__) {
    if (parameters_) {
      parameters_->setFather(astFuncCall);
    }
  }
  return astFuncCall;
}

std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
Builder::createFuncDecl(const std::string &id, const Parameters &params,
                        const std::string &returnType,
                        const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astFuncDecl{
      std::make_shared<AST_FUNC_DECL>(id, params, returnType, body)};
  if (body) {
    body->setFather(astFuncDecl);
  }
  return astFuncDecl;
}

std::expected<std::shared_ptr<AST_RETURN>, Error>
Builder::createReturn(const std::shared_ptr<AST> &value) noexcept {
  const auto astReturn{std::make_shared<AST_RETURN>(value)};
  if (value) {
    value->setFather(astReturn);
  }
  return astReturn;
}

std::expected<std::shared_ptr<AST_ENUM>, Error>
Builder::createEnum(const std::string &id,
                    const std::vector<std::string> &enumIdentifiers) noexcept {
  const auto astEnum{std::make_shared<AST_ENUM>(id, enumIdentifiers)};
  return astEnum;
}

std::expected<std::shared_ptr<AST_STRUCT>, Error> Builder::createStruct(
    const std::string &id, std::unique_ptr<std::string> fatherType,
    const Attributes &attributes,
    const std::vector<std::shared_ptr<AST_METHOD_DECL>> &methods,
    const std::shared_ptr<AST_CONSTRUCTOR_DECL> &constructor,
    const std::shared_ptr<AST_DESTRUCTOR_DECL> &destructor) noexcept {
  const auto astStruct{std::make_shared<AST_STRUCT>(
      id, std::move(fatherType), attributes, methods, constructor, destructor)};
  const std::vector<std::shared_ptr<AST_METHOD_DECL>> &methods__{
      astStruct->methods()};
  for (const auto &methods_ : methods__) {
    if (methods_) {
      methods_->setFather(astStruct);
    }
  }
  if (constructor) {
    constructor->setFather(astStruct);
  }
  if (destructor) {
    destructor->setFather(astStruct);
  }
  return astStruct;
}

std::expected<std::shared_ptr<AST_CLASS>, Error> Builder::createClass(
    const std::string &id, std::unique_ptr<std::string> fatherType,
    const Attributes &attributes,
    const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods,
    const std::shared_ptr<AST_FUNC_DECL> &constructor,
    const std::shared_ptr<AST_FUNC_DECL> &destructor) noexcept {
  const auto astClass{std::make_shared<AST_CLASS>(
      id, std::move(fatherType), attributes, methods, constructor, destructor)};
  const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods__{
      astClass->methods()};
  for (const auto &methods_ : methods__) {
    if (methods_) {
      methods_->setFather(astClass);
    }
  }
  if (constructor) {
    constructor->setFather(astClass);
  }
  if (destructor) {
    destructor->setFather(astClass);
  }
  return astClass;
}

std::expected<std::shared_ptr<AST_ATTR_ACCESS>, Error>
Builder::createAttrAccess(const std::string &id) noexcept {
  return std::make_shared<AST_ATTR_ACCESS>(id);
}

std::expected<std::shared_ptr<AST_METHOD_CALL>, Error>
Builder::createMethodCall(
    const std::string &id,
    const std::vector<std::shared_ptr<AST>> &parameters) noexcept {
  const auto astMethodCall{std::make_shared<AST_METHOD_CALL>(id, parameters)};
  const std::vector<std::shared_ptr<AST>> &parameters__{
      astMethodCall->parameters()};
  for (const auto &parameters_ : parameters__) {
    if (parameters_) {
      parameters_->setFather(astMethodCall);
    }
  }
  return astMethodCall;
}

std::expected<std::shared_ptr<AST_METHOD_DECL>, Error>
Builder::createMethodDecl(const std::string &id, const Parameters &params,
                          const std::string &returnType,
                          const std::shared_ptr<AST_BODY> &body,
                          const bool isVirtual) noexcept {
  const auto astMethodDecl{std::make_shared<AST_METHOD_DECL>(
      id, params, returnType, body, isVirtual)};
  if (body) {
    body->setFather(astMethodDecl);
  }
  return astMethodDecl;
}

std::expected<std::shared_ptr<AST_CONSTRUCTOR_CALL>, Error>
Builder::createConstructorCall(
    const std::string &id,
    const std::vector<std::shared_ptr<AST>> &parameters) noexcept {
  const auto astConstructorall{
      std::make_shared<AST_CONSTRUCTOR_CALL>(id, parameters)};
  const std::vector<std::shared_ptr<AST>> &parameters__{
      astConstructorall->parameters()};
  for (const auto &parameters_ : parameters__) {
    if (parameters_) {
      parameters_->setFather(astConstructorall);
    }
  }
  return astConstructorall;
}

std::expected<std::shared_ptr<AST_CONSTRUCTOR_DECL>, Error>
Builder::createConstructorDecl(const std::string &id_returnType,
                               const Parameters &params,
                               const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astConstructor{
      std::make_shared<AST_CONSTRUCTOR_DECL>(id_returnType, params, body)};
  if (body) {
    body->setFather(astConstructor);
  }
  return astConstructor;
}

std::expected<std::shared_ptr<AST_DESTRUCTOR_DECL>, Error>
Builder::createDestructorDecl(const std::string &id,
                              const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astConstructor{std::make_shared<AST_DESTRUCTOR_DECL>(id, body)};
  if (body) {
    body->setFather(astConstructor);
  }
  return astConstructor;
}

std::expected<std::shared_ptr<AST_THIS>, Error> Builder::createThis() noexcept {
  const auto astThis{std::make_shared<AST_THIS>()};
  return astThis;
}

std::expected<std::shared_ptr<AST_AUTO_DECL>, Error>
Builder::createAutoDecl(const std::string &id,
                        const std::shared_ptr<AST> &value,
                        const bool isConst) noexcept {
  const auto astAutoDecl{std::make_shared<AST_AUTO_DECL>(id, value, isConst)};
  if (value) {
    value->setFather(astAutoDecl);
  }
  return astAutoDecl;
}

std::expected<std::shared_ptr<AST_VAR_TYPED_DECL>, Error>
Builder::createVarTypedtDecl(const std::string &id, const std::string &type,
                             const std::shared_ptr<AST> &value,
                             const bool isConst) noexcept {
  const auto astLetDecl{
      std::make_shared<AST_VAR_TYPED_DECL>(id, type, value, isConst)};
  if (value) {
    value->setFather(astLetDecl);
  }
  return astLetDecl;
}

std::expected<std::shared_ptr<AST_VAR_CALL>, Error>
Builder::createVarCall(const std::string &id) noexcept {
  return std::make_shared<AST_VAR_CALL>(id);
}

std::expected<std::shared_ptr<AST_CHAINED>, Error> Builder::createChained(
    const std::shared_ptr<AST> &base,
    const std::vector<std::shared_ptr<AST>> &operations) noexcept {
  const auto astChained{std::make_shared<AST_CHAINED>(base, operations)};
  if (base) {
    base->setFather(astChained);

    const std::vector<std::shared_ptr<AST>> &operations__{
        astChained->operations()};

    if (operations__.size()) {
      operations__[0]->setFather(base);

      const std::size_t size{operations__.size()};
      if (size > 1) {
        for (std::size_t i{1}; i < size; ++i) {
          if (operations__[i] and operations__[i - 1]) {
            operations__[i]->setFather(operations__[i - 1]);
          }
        }
      }
    }
  }
  return astChained;
}

std::expected<std::shared_ptr<Tree>, Error>
Builder::createTree(const std::shared_ptr<AST_BODY> &statements) noexcept {
  return std::make_shared<Tree>(statements);
}

} // namespace nicole