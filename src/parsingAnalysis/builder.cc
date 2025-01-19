#include "../../inc/parsingAnalysis/builder.h"
#include <cstddef>

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
Builder::createVector(const std::string type,
                      const std::vector<std::shared_ptr<AST>> values) noexcept {
  const auto astVector{std::make_shared<AST_VECTOR>(type, values)};
  const std::vector<std::shared_ptr<AST>> &statements{astVector->values()};
  for (const auto &statement : statements) {
    statement->setFather(astVector);
  }
  return astVector;
}

std::expected<std::shared_ptr<AST_DELETE>, Error>
Builder::createDelete(const std::shared_ptr<AST> &value) noexcept {
  const auto astDelete{std::make_shared<AST_DELETE>(value)};
  value->setFather(astDelete);
  return astDelete;
}

std::expected<std::shared_ptr<AST_NEW>, Error>
Builder::createNew(const std::shared_ptr<AST> &value) noexcept {
  const auto astNew{std::make_shared<AST_NEW>(value)};
  value->setFather(astNew);
  return astNew;
}

std::expected<std::shared_ptr<AST_DEREF>, Error>
Builder::createDeref(const std::shared_ptr<AST> &value) noexcept {
  const auto astDeref{std::make_shared<AST_DEREF>(value)};
  value->setFather(astDeref);
  return astDeref;
}

std::expected<std::shared_ptr<AST_PTR>, Error>
Builder::createPtr(const std::string &id, const std::string &type,
                   const std::shared_ptr<AST> &value) noexcept {
  const auto astPtr{std::make_shared<AST_PTR>(id, type, value)};
  value->setFather(astPtr);
  return astPtr;
}

std::expected<std::shared_ptr<AST_ADD>, Error>
Builder::createAdd(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right) noexcept {
  const auto astAdd{std::make_shared<AST_ADD>(op, left, right)};
  left->setFather(astAdd);
  right->setFather(astAdd);
  return astAdd;
}

std::expected<std::shared_ptr<AST_SUB>, Error>
Builder::createSub(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right) noexcept {
  const auto astSub{std::make_shared<AST_SUB>(op, left, right)};
  left->setFather(astSub);
  right->setFather(astSub);
  return astSub;
}

std::expected<std::shared_ptr<AST_MULT>, Error>
Builder::createMult(const Token &op, const std::shared_ptr<AST> &left,
                    const std::shared_ptr<AST> &right) noexcept {
  const auto astMult{std::make_shared<AST_MULT>(op, left, right)};
  left->setFather(astMult);
  right->setFather(astMult);
  return astMult;
}

std::expected<std::shared_ptr<AST_DIV>, Error>
Builder::createDiv(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right) noexcept {
  const auto astDiv{std::make_shared<AST_DIV>(op, left, right)};
  left->setFather(astDiv);
  right->setFather(astDiv);
  return astDiv;
}

std::expected<std::shared_ptr<AST_MODULE>, Error>
Builder::createModule(const Token &op, const std::shared_ptr<AST> &left,
                      const std::shared_ptr<AST> &right) noexcept {
  const auto astModule{std::make_shared<AST_MODULE>(op, left, right)};
  left->setFather(astModule);
  right->setFather(astModule);
  return astModule;
}

std::expected<std::shared_ptr<AST_SMALLER>, Error>
Builder::createSmaller(const Token &op, const std::shared_ptr<AST> &left,
                       const std::shared_ptr<AST> &right) noexcept {
  const auto astSmaller{std::make_shared<AST_SMALLER>(op, left, right)};
  left->setFather(astSmaller);
  right->setFather(astSmaller);
  return astSmaller;
}

std::expected<std::shared_ptr<AST_SMALLER_EQUAL>, Error>
Builder::createSmallerEqual(const Token &op, const std::shared_ptr<AST> &left,
                            const std::shared_ptr<AST> &right) noexcept {
  const auto astSmallerEqual{
      std::make_shared<AST_SMALLER_EQUAL>(op, left, right)};
  left->setFather(astSmallerEqual);
  right->setFather(astSmallerEqual);
  return astSmallerEqual;
}

std::expected<std::shared_ptr<AST_BIGGER>, Error>
Builder::createBigger(const Token &op, const std::shared_ptr<AST> &left,
                      const std::shared_ptr<AST> &right) noexcept {
  const auto astBigger{std::make_shared<AST_BIGGER>(op, left, right)};
  left->setFather(astBigger);
  right->setFather(astBigger);
  return astBigger;
}

std::expected<std::shared_ptr<AST_BIGGER_EQUAL>, Error>
Builder::createBiggerEqual(const Token &op, const std::shared_ptr<AST> &left,
                           const std::shared_ptr<AST> &right) noexcept {
  const auto astBiggerEqual{
      std::make_shared<AST_BIGGER_EQUAL>(op, left, right)};
  left->setFather(astBiggerEqual);
  right->setFather(astBiggerEqual);
  return astBiggerEqual;
}

std::expected<std::shared_ptr<AST_EQUAL>, Error>
Builder::createEqual(const Token &op, const std::shared_ptr<AST> &left,
                     const std::shared_ptr<AST> &right) noexcept {
  const auto astEqual{std::make_shared<AST_EQUAL>(op, left, right)};
  left->setFather(astEqual);
  right->setFather(astEqual);
  return astEqual;
}

std::expected<std::shared_ptr<AST_NOT_EQUAL>, Error>
Builder::createNotEqual(const Token &op, const std::shared_ptr<AST> &left,
                        const std::shared_ptr<AST> &right) noexcept {
  const auto astNotEqual{std::make_shared<AST_NOT_EQUAL>(op, left, right)};
  left->setFather(astNotEqual);
  right->setFather(astNotEqual);
  return astNotEqual;
}

std::expected<std::shared_ptr<AST_OR>, Error>
Builder::createOr(const Token &op, const std::shared_ptr<AST> &left,
                  const std::shared_ptr<AST> &right) noexcept {
  const auto astOr{std::make_shared<AST_OR>(op, left, right)};
  left->setFather(astOr);
  right->setFather(astOr);
  return astOr;
}

std::expected<std::shared_ptr<AST_AND>, Error>
Builder::createAnd(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right) noexcept {
  const auto astAnd{std::make_shared<AST_AND>(op, left, right)};
  left->setFather(astAnd);
  right->setFather(astAnd);
  return astAnd;
}

std::expected<std::shared_ptr<AST_NEG>, Error>
Builder::createNeg(const Token &op,
                   const std::shared_ptr<AST> &value) noexcept {
  const auto astNeg{std::make_shared<AST_NEG>(op, value)};
  value->setFather(astNeg);
  return astNeg;
}

std::expected<std::shared_ptr<AST_NOT>, Error>
Builder::createNot(const Token &op,
                   const std::shared_ptr<AST> &value) noexcept {
  const auto astNot{std::make_shared<AST_NOT>(op, value)};
  value->setFather(astNot);
  return astNot;
}

std::expected<std::shared_ptr<AST_INCREMENT>, Error>
Builder::createIncrement(const Token &op,
                         const std::shared_ptr<AST> &value) noexcept {
  const auto astIncrement{std::make_shared<AST_INCREMENT>(op, value)};
  value->setFather(astIncrement);
  return astIncrement;
}

std::expected<std::shared_ptr<AST_DECREMENT>, Error>
Builder::createDecrement(const Token &op,
                         const std::shared_ptr<AST> &value) noexcept {
  const auto astDecrement{std::make_shared<AST_DECREMENT>(op, value)};
  value->setFather(astDecrement);
  return astDecrement;
}

std::expected<std::shared_ptr<AST_ASSIGNMENT>, Error>
Builder::createAssignment(const std::string &id,
                          const std::shared_ptr<AST> &value) noexcept {
  const auto astAssignment{std::make_shared<AST_ASSIGNMENT>(id, value)};
  value->setFather(astAssignment);
  return astAssignment;
}

std::expected<std::shared_ptr<AST_SELF_ADD>, Error>
Builder::createSelfAdd(const std::string &id,
                       const std::shared_ptr<AST> &value) noexcept {
  const auto astSelfAdd{std::make_shared<AST_SELF_ADD>(id, value)};
  value->setFather(astSelfAdd);
  return astSelfAdd;
}

std::expected<std::shared_ptr<AST_SELF_SUB>, Error>
Builder::createSelfSub(const std::string &id,
                       const std::shared_ptr<AST> &value) noexcept {
  const auto astSelfSub{std::make_shared<AST_SELF_SUB>(id, value)};
  value->setFather(astSelfSub);
  return astSelfSub;
}

std::expected<std::shared_ptr<AST_SELF_MULT>, Error>
Builder::createSelfMult(const std::string &id,
                        const std::shared_ptr<AST> &value) noexcept {
  const auto astSelfMult{std::make_shared<AST_SELF_MULT>(id, value)};
  value->setFather(astSelfMult);
  return astSelfMult;
}

std::expected<std::shared_ptr<AST_SELF_DIV>, Error>
Builder::createSelfDiv(const std::string &id,
                       const std::shared_ptr<AST> &value) noexcept {
  const auto astSelfDiv{std::make_shared<AST_SELF_DIV>(id, value)};
  value->setFather(astSelfDiv);
  return astSelfDiv;
}

std::expected<std::shared_ptr<AST_PRINT>, Error>
Builder::createPrint(const std::vector<std::shared_ptr<AST>> &values) noexcept {
  const auto astPrint{std::make_shared<AST_PRINT>(values)};
  const std::vector<std::shared_ptr<AST>> &values__{astPrint->values()};
  for (const auto &values_ : values__) {
    values_->setFather(astPrint);
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
  expression->setFather(astStatement);
  return astStatement;
}

std::expected<std::shared_ptr<AST_BODY>, Error> Builder::createBody(
    const std::vector<std::shared_ptr<AST_STATEMENT>> &body) noexcept {
  const auto astBody{std::make_shared<AST_BODY>(body)};
  const std::vector<std::shared_ptr<AST_STATEMENT>> &statements{
      astBody->body()};
  for (const auto &statement : statements) {
    statement->setFather(astBody);
  }
  return astBody;
}

std::expected<std::shared_ptr<AST_COMMA>, Error> Builder::createCOMMA(
    const std::vector<std::shared_ptr<AST_STATEMENT>> &body) noexcept {
  const auto astComma{std::make_shared<AST_COMMA>(body)};
  const std::vector<std::shared_ptr<AST_STATEMENT>> &statements{
      astComma->body()};
  for (const auto &statement : statements) {
    statement->setFather(astComma);
  }
  return astComma;
}

std::expected<std::shared_ptr<AST_WHILE>, Error>
Builder::createWhile(const std::shared_ptr<AST> &condition,
                     const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astWhile{std::make_shared<AST_WHILE>(condition, body)};
  condition->setFather(astWhile);
  body->setFather(astWhile);
  return astWhile;
}

std::expected<std::shared_ptr<AST_FOR>, Error>
Builder::createFor(const std::shared_ptr<AST_COMMA> &init,
                   const std::shared_ptr<AST> &condition,
                   const std::shared_ptr<AST_COMMA> &update,
                   const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astFor{std::make_shared<AST_FOR>(init, condition, update, body)};
  init->setFather(astFor);
  condition->setFather(astFor);
  update->setFather(astFor);
  body->setFather(astFor);
  return astFor;
}

std::expected<std::shared_ptr<AST_DO_WHILE>, Error>
Builder::createDoWhile(const std::shared_ptr<AST_BODY> &body,
                       const std::shared_ptr<AST> &condition) noexcept {
  const auto astDoWhile{std::make_shared<AST_DO_WHILE>(body, condition)};
  condition->setFather(astDoWhile);
  body->setFather(astDoWhile);
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
Builder::createIf(const std::shared_ptr<AST> &condition,
                  const std::shared_ptr<AST_BODY> &body,
                  const std::vector<std::shared_ptr<AST_ELSE_IF>> &elseIf,
                  const std::shared_ptr<AST_BODY> &elseBody) noexcept {
  const auto astIf{std::make_shared<AST_IF>(condition, body, elseIf, elseBody)};
  condition->setFather(astIf);
  body->setFather(astIf);
  const std::vector<std::shared_ptr<AST_ELSE_IF>> &elseIfs{astIf->elseIf()};
  for (const auto &elseIf_ : elseIfs) {
    elseIf_->setFather(astIf);
  }
  elseBody->setFather(astIf);
  return astIf;
}

std::expected<std::shared_ptr<AST_ELSE_IF>, Error>
Builder::createElseIf(const std::shared_ptr<AST> &condition,
                      const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astIf{std::make_shared<AST_ELSE_IF>(condition, body)};
  condition->setFather(astIf);
  body->setFather(astIf);
  return astIf;
}

std::expected<std::shared_ptr<AST_SWITCH>, Error>
Builder::createSwitch(const std::shared_ptr<AST> &condition,
                      const std::vector<std::shared_ptr<AST_CASE>> &cases,
                      const std::shared_ptr<AST_DEFAULT> &default_) noexcept {
  const auto astSwitch{
      std::make_shared<AST_SWITCH>(condition, cases, default_)};
  condition->setFather(astSwitch);
  const std::vector<std::shared_ptr<AST_CASE>> &cases_{astSwitch->cases()};
  for (const auto &statement : cases_) {
    statement->setFather(astSwitch);
  }
  default_->setFather(astSwitch);
  return astSwitch;
}

std::expected<std::shared_ptr<AST_CASE>, Error>
Builder::createCase(const std::shared_ptr<AST> &match,
                    const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astCase{std::make_shared<AST_CASE>(match, body)};
  match->setFather(astCase);
  body->setFather(astCase);
  return astCase;
}

std::expected<std::shared_ptr<AST_DEFAULT>, Error>
Builder::createDefault(const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astDefault{std::make_shared<AST_DEFAULT>(body)};
  body->setFather(astDefault);
  return astDefault;
}

std::expected<std::shared_ptr<AST_TERNARY>, Error>
Builder::createTernary(const std::shared_ptr<AST> &condition,
                       const std::shared_ptr<AST> &first,
                       const std::shared_ptr<AST> &second) noexcept {
  const auto astTernanry{
      std::make_shared<AST_TERNARY>(condition, first, second)};
  condition->setFather(astTernanry);
  first->setFather(astTernanry);
  second->setFather(astTernanry);
  return astTernanry;
}

std::expected<std::shared_ptr<AST_FUNC_CALL>, Error> Builder::createFunCall(
    const std::string &id,
    const std::vector<std::shared_ptr<AST>> &parameters) noexcept {
  const auto astFuncCall{std::make_shared<AST_FUNC_CALL>(id, parameters)};
  const std::vector<std::shared_ptr<AST>> &parameters__{
      astFuncCall->parameters()};
  for (const auto &parameters_ : parameters__) {
    parameters_->setFather(astFuncCall);
  }
  return astFuncCall;
}

std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
Builder::createFuncDecl(const std::string &id, const Parameters &params,
                        const std::string &returnType,
                        const std::shared_ptr<AST_BODY> &body) noexcept {
  const auto astFuncDecl{
      std::make_shared<AST_FUNC_DECL>(id, params, returnType, body)};
  body->setFather(astFuncDecl);
  return astFuncDecl;
}

std::expected<std::shared_ptr<AST_RETURN>, Error>
Builder::createReturn(const std::shared_ptr<AST> &value) noexcept {
  const auto astReturn{std::make_shared<AST_RETURN>(value)};
  value->setFather(astReturn);
  return astReturn;
}

std::expected<std::shared_ptr<AST_ENUM>, Error>
Builder::createEnum(const std::string &id,
                    const std::vector<std::string> &enumIdentifiers) noexcept {
  const auto astEnum{std::make_shared<AST_ENUM>(id, enumIdentifiers)};
  return astEnum;
}

std::expected<std::shared_ptr<AST_STRUCT>, Error> Builder::createStruct(
    const std::string &id, const Attributes &attributes,
    const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods,
    const std::shared_ptr<AST_FUNC_DECL> &constructor,
    const std::shared_ptr<AST_FUNC_DECL> &destructor,
    const std::shared_ptr<AST_FUNC_DECL> &addOverloading) noexcept {
  const auto astStruct{std::make_shared<AST_STRUCT>(
      id, attributes, methods, constructor, destructor, addOverloading)};
  const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods__{
      astStruct->methods()};
  for (const auto &methods_ : methods__) {
    methods_->setFather(astStruct);
  }
  constructor->setFather(astStruct);
  destructor->setFather(astStruct);
  addOverloading->setFather(astStruct);
  return astStruct;
}

std::expected<std::shared_ptr<AST_CLASS>, Error> Builder::createClass(
    const std::string &id, const Attributes &attributes,
    const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods,
    const std::shared_ptr<AST_FUNC_DECL> &constructor,
    const std::shared_ptr<AST_FUNC_DECL> &destructor,
    const std::shared_ptr<AST_FUNC_DECL> &addOverloading) noexcept {
  const auto astClass{std::make_shared<AST_CLASS>(
      id, attributes, methods, constructor, destructor, addOverloading)};
  const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods__{
      astClass->methods()};
  for (const auto &methods_ : methods__) {
    methods_->setFather(astClass);
  }
  constructor->setFather(astClass);
  destructor->setFather(astClass);
  addOverloading->setFather(astClass);
  return astClass;
}

std::expected<std::shared_ptr<AST_AUTO_DECL>, Error>
Builder::createAutoDecl(const std::string &id,
                        const std::shared_ptr<AST> &value) noexcept {
  const auto astAutoDecl{std::make_shared<AST_AUTO_DECL>(id, value)};
  value->setFather(astAutoDecl);
  return astAutoDecl;
}

std::expected<std::shared_ptr<AST_CONST_DECL>, Error>
Builder::createConstDecl(const std::string &id, const std::string &type,
                         const std::shared_ptr<AST> &value) noexcept {
  const auto astConstDecl{std::make_shared<AST_CONST_DECL>(id, type, value)};
  value->setFather(astConstDecl);
  return astConstDecl;
}

std::expected<std::shared_ptr<AST_LET_DECL>, Error>
Builder::createLetDecl(const std::string &id, const std::string &type,
                       const std::shared_ptr<AST> &value) noexcept {
  const auto astLetDecl{std::make_shared<AST_LET_DECL>(id, type, value)};
  value->setFather(astLetDecl);
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

  base->setFather(astChained);

  const std::vector<std::shared_ptr<AST>> &operations__{
      astChained->operations()};

  operations__[0]->setFather(base);

  const std::size_t size{operations__.size()};
  for (std::size_t i{1}; i < size; ++i) {
    operations__[i]->setFather(operations__[i - 1]);
  }

  return astChained;
}

std::expected<std::shared_ptr<Tree>, Error>
Builder::createTree(const std::shared_ptr<AST_BODY> &statements) noexcept {
  return std::make_shared<Tree>(statements);
}

} // namespace nicole