#include "../../inc/parsingAnalysis/builder.h"

namespace nicole {

std::expected<std::shared_ptr<AST_BOOL>, Error>
Builder::createBool(const bool value,
                    const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_BOOL>(value, sourceLocation);
}

std::expected<std::shared_ptr<AST_CHAR>, Error>
Builder::createChar(const char value,
                    const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_CHAR>(value, sourceLocation);
}

std::expected<std::shared_ptr<AST_DOUBLE>, Error>
Builder::createDouble(const double value,
                      const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_DOUBLE>(value, sourceLocation);
}

std::expected<std::shared_ptr<AST_FLOAT>, Error>
Builder::createFloat(const float value,
                     const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_FLOAT>(value, sourceLocation);
}

std::expected<std::shared_ptr<AST_INT>, Error>
Builder::createInt(const int value,
                   const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_INT>(value, sourceLocation);
}

std::expected<std::shared_ptr<AST_NULL>, Error>
Builder::createNull(const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_NULL>(sourceLocation);
}

std::expected<std::shared_ptr<AST_STRING>, Error>
Builder::createString(const std::string value,
                      const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_STRING>(value, sourceLocation);
}

std::expected<std::shared_ptr<AST_ADD>, Error>
Builder::createAdd(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation) noexcept {
  const auto astAdd{std::make_shared<AST_ADD>(op, left, right, sourceLocation)};
  left->setFather(astAdd);
  right->setFather(astAdd);
  return astAdd;
}

std::expected<std::shared_ptr<AST_SUB>, Error>
Builder::createSub(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation) noexcept {
  const auto astSub{std::make_shared<AST_SUB>(op, left, right, sourceLocation)};
  left->setFather(astSub);
  right->setFather(astSub);
  return astSub;
}

std::expected<std::shared_ptr<AST_MULT>, Error>
Builder::createMult(const Token &op, const std::shared_ptr<AST> &left,
                    const std::shared_ptr<AST> &right,
                    const SourceLocation &sourceLocation) noexcept {
  const auto astMult{
      std::make_shared<AST_MULT>(op, left, right, sourceLocation)};
  left->setFather(astMult);
  right->setFather(astMult);
  return astMult;
}

std::expected<std::shared_ptr<AST_DIV>, Error>
Builder::createDiv(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation) noexcept {
  const auto astDiv{std::make_shared<AST_DIV>(op, left, right, sourceLocation)};
  left->setFather(astDiv);
  right->setFather(astDiv);
  return astDiv;
}

std::expected<std::shared_ptr<AST_MODULE>, Error>
Builder::createModule(const Token &op, const std::shared_ptr<AST> &left,
                      const std::shared_ptr<AST> &right,
                      const SourceLocation &sourceLocation) noexcept {
  const auto astModule{
      std::make_shared<AST_MODULE>(op, left, right, sourceLocation)};
  left->setFather(astModule);
  right->setFather(astModule);
  return astModule;
}

std::expected<std::shared_ptr<AST_SMALLER>, Error>
Builder::createSmaller(const Token &op, const std::shared_ptr<AST> &left,
                       const std::shared_ptr<AST> &right,
                       const SourceLocation &sourceLocation) noexcept {
  const auto astSmaller{
      std::make_shared<AST_SMALLER>(op, left, right, sourceLocation)};
  left->setFather(astSmaller);
  right->setFather(astSmaller);
  return astSmaller;
}

std::expected<std::shared_ptr<AST_SMALLER_EQUAL>, Error>
Builder::createSmallerEqual(const Token &op, const std::shared_ptr<AST> &left,
                            const std::shared_ptr<AST> &right,
                            const SourceLocation &sourceLocation) noexcept {
  const auto astSmallerEqual{
      std::make_shared<AST_SMALLER_EQUAL>(op, left, right, sourceLocation)};
  left->setFather(astSmallerEqual);
  right->setFather(astSmallerEqual);
  return astSmallerEqual;
}

std::expected<std::shared_ptr<AST_BIGGER>, Error>
Builder::createBigger(const Token &op, const std::shared_ptr<AST> &left,
                      const std::shared_ptr<AST> &right,
                      const SourceLocation &sourceLocation) noexcept {
  const auto astBigger{
      std::make_shared<AST_BIGGER>(op, left, right, sourceLocation)};
  left->setFather(astBigger);
  right->setFather(astBigger);
  return astBigger;
}

std::expected<std::shared_ptr<AST_BIGGER_EQUAL>, Error>
Builder::createBiggerEqual(const Token &op, const std::shared_ptr<AST> &left,
                           const std::shared_ptr<AST> &right,
                           const SourceLocation &sourceLocation) noexcept {
  const auto astBiggerEqual{
      std::make_shared<AST_BIGGER_EQUAL>(op, left, right, sourceLocation)};
  left->setFather(astBiggerEqual);
  right->setFather(astBiggerEqual);
  return astBiggerEqual;
}

std::expected<std::shared_ptr<AST_EQUAL>, Error>
Builder::createEqual(const Token &op, const std::shared_ptr<AST> &left,
                     const std::shared_ptr<AST> &right,
                     const SourceLocation &sourceLocation) noexcept {
  const auto astEqual{
      std::make_shared<AST_EQUAL>(op, left, right, sourceLocation)};
  left->setFather(astEqual);
  right->setFather(astEqual);
  return astEqual;
}

std::expected<std::shared_ptr<AST_NOT_EQUAL>, Error>
Builder::createNotEqual(const Token &op, const std::shared_ptr<AST> &left,
                        const std::shared_ptr<AST> &right,
                        const SourceLocation &sourceLocation) noexcept {
  const auto astNotEqual{
      std::make_shared<AST_NOT_EQUAL>(op, left, right, sourceLocation)};
  left->setFather(astNotEqual);
  right->setFather(astNotEqual);
  return astNotEqual;
}

std::expected<std::shared_ptr<AST_OR>, Error>
Builder::createOr(const Token &op, const std::shared_ptr<AST> &left,
                  const std::shared_ptr<AST> &right,
                  const SourceLocation &sourceLocation) noexcept {
  const auto astOr{std::make_shared<AST_OR>(op, left, right, sourceLocation)};
  left->setFather(astOr);
  right->setFather(astOr);
  return astOr;
}

std::expected<std::shared_ptr<AST_AND>, Error>
Builder::createAnd(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation) noexcept {
  const auto astAnd{std::make_shared<AST_AND>(op, left, right, sourceLocation)};
  left->setFather(astAnd);
  right->setFather(astAnd);
  return astAnd;
}

std::expected<std::shared_ptr<AST_NEG>, Error>
Builder::createNeg(const Token &op, const std::shared_ptr<AST> &value,
          const SourceLocation &sourceLocation) noexcept {
  const auto astNeg{std::make_shared<AST_NEG>(op, value, sourceLocation)};
  value->setFather(astNeg);
  return astNeg;
}

std::expected<std::shared_ptr<AST_NOT>, Error>
Builder::createNot(const Token &op, const std::shared_ptr<AST> &value,
          const SourceLocation &sourceLocation) noexcept {
  const auto astNot{std::make_shared<AST_NOT>(op, value, sourceLocation)};
  value->setFather(astNot);
  return astNot;
}

std::expected<std::shared_ptr<AST_INCREMENT>, Error>
Builder::createIncrement(const Token &op, const std::shared_ptr<AST> &value,
                const SourceLocation &sourceLocation) noexcept {
  const auto astIncrement{
      std::make_shared<AST_INCREMENT>(op, value, sourceLocation)};
  value->setFather(astIncrement);
  return astIncrement;
}

std::expected<std::shared_ptr<AST_DECREMENT>, Error>
Builder::createDecrement(const Token &op, const std::shared_ptr<AST> &value,
                const SourceLocation &sourceLocation) noexcept {
  const auto astDecrement{
      std::make_shared<AST_DECREMENT>(op, value, sourceLocation)};
  value->setFather(astDecrement);
  return astDecrement;
}

std::expected<std::shared_ptr<AST_ASSIGNMENT>, Error>
Builder::createAssignment(const std::string &id, const std::shared_ptr<AST> &value,
                 const SourceLocation &sourceLocation) noexcept {
  const auto astAssignment{
      std::make_shared<AST_ASSIGNMENT>(id, value, sourceLocation)};
  value->setFather(astAssignment);
  return astAssignment;
}

std::expected<std::shared_ptr<AST_SELF_ADD>, Error>
Builder::createSelfAdd(const std::string &id, const std::shared_ptr<AST> &value,
              const SourceLocation &sourceLocation) noexcept {
  const auto astSelfAdd{
      std::make_shared<AST_SELF_ADD>(id, value, sourceLocation)};
  value->setFather(astSelfAdd);
  return astSelfAdd;
}

std::expected<std::shared_ptr<AST_SELF_SUB>, Error>
Builder::createSelfSub(const std::string &id, const std::shared_ptr<AST> &value,
              const SourceLocation &sourceLocation) noexcept {
  const auto astSelfSub{
      std::make_shared<AST_SELF_SUB>(id, value, sourceLocation)};
  value->setFather(astSelfSub);
  return astSelfSub;
}

std::expected<std::shared_ptr<AST_SELF_MULT>, Error>
Builder::createSelfMult(const std::string &id, const std::shared_ptr<AST> &value,
               const SourceLocation &sourceLocation) noexcept {
  const auto astSelfMult{
      std::make_shared<AST_SELF_MULT>(id, value, sourceLocation)};
  value->setFather(astSelfMult);
  return astSelfMult;
}

std::expected<std::shared_ptr<AST_SELF_DIV>, Error>
Builder::createSelfDiv(const std::string &id, const std::shared_ptr<AST> &value,
              const SourceLocation &sourceLocation) noexcept {
  const auto astSelfDiv{
      std::make_shared<AST_SELF_DIV>(id, value, sourceLocation)};
  value->setFather(astSelfDiv);
  return astSelfDiv;
}

std::expected<std::shared_ptr<AST_PRINT>, Error>
Builder::createPrint(const std::shared_ptr<AST> &value,
            const SourceLocation &sourceLocation) noexcept {
  const auto astPrint{std::make_shared<AST_PRINT>(value, sourceLocation)};
  value->setFather(astPrint);
  return astPrint;
}

std::expected<std::shared_ptr<AST_IMPORT>, Error>
Builder::createImport(const std::filesystem::path &path,
             const SourceLocation &sourceLocation) noexcept {
  const auto astImport{std::make_shared<AST_IMPORT>(path, sourceLocation)};
  return astImport;
}

std::expected<std::shared_ptr<AST_STATEMENT>, Error>
Builder::createStatement(const std::shared_ptr<AST> &expression,
                const SourceLocation &sourceLocation) noexcept {
  const auto astStatement{
      std::make_shared<AST_STATEMENT>(expression, sourceLocation)};
  expression->setFather(astStatement);
  return astStatement;
}

std::expected<std::shared_ptr<AST_BODY>, Error>
Builder::createBody(const std::vector<std::shared_ptr<AST_STATEMENT>> &body,
           const SourceLocation &sourceLocation) noexcept {
  const auto astBody{std::make_shared<AST_BODY>(body, sourceLocation)};
  const std::vector<std::shared_ptr<AST_STATEMENT>> &statements{
      astBody->body()};
  for (const auto &statement : statements) {
    statement->setFather(astBody);
  }
  return astBody;
}

std::expected<std::shared_ptr<AST_COMMA>, Error>
Builder::createCOMMA(const std::vector<std::shared_ptr<AST_STATEMENT>> &body,
            const SourceLocation &sourceLocation) noexcept {
  const auto astComma{std::make_shared<AST_COMMA>(body, sourceLocation)};
  const std::vector<std::shared_ptr<AST_STATEMENT>> &statements{
      astComma->body()};
  for (const auto &statement : statements) {
    statement->setFather(astComma);
  }
  return astComma;
}

std::expected<std::shared_ptr<AST_WHILE>, Error>
Builder::createWhile(const std::shared_ptr<AST> &condition,
            const std::shared_ptr<AST_BODY> &body,
            const SourceLocation &sourceLocation) noexcept {
  const auto astWhile{
      std::make_shared<AST_WHILE>(condition, body, sourceLocation)};
  condition->setFather(astWhile);
  body->setFather(astWhile);
  return astWhile;
}

std::expected<std::shared_ptr<AST_FOR>, Error>
Builder::createFor(const std::shared_ptr<AST_COMMA> &init,
          const std::shared_ptr<AST> &condition,
          const std::shared_ptr<AST_COMMA> &update,
          const std::shared_ptr<AST_BODY> &body,
          const SourceLocation &sourceLocation) noexcept {
  const auto astFor{
      std::make_shared<AST_FOR>(init, condition, update, body, sourceLocation)};
  init->setFather(astFor);
  condition->setFather(astFor);
  update->setFather(astFor);
  body->setFather(astFor);
  return astFor;
}

std::expected<std::shared_ptr<AST_DO_WHILE>, Error>
Builder::createDoWhile(const std::shared_ptr<AST_BODY> &body,
              const std::shared_ptr<AST> &condition,
              const SourceLocation &sourceLocation) noexcept {
  const auto astDoWhile{
      std::make_shared<AST_DO_WHILE>(body, condition, sourceLocation)};
  condition->setFather(astDoWhile);
  body->setFather(astDoWhile);
  return astDoWhile;
}

std::expected<std::shared_ptr<AST_PASS>, Error>
Builder::createPass(const std::shared_ptr<AST> &fatherLoop,
           const SourceLocation &sourceLocation) noexcept {
  const auto astPass{std::make_shared<AST_PASS>(fatherLoop, sourceLocation)};
  return astPass;
}

std::expected<std::shared_ptr<AST_STOP>, Error>
Builder::createStop(const std::shared_ptr<AST> &fatherLoop,
           const SourceLocation &sourceLocation) noexcept {
  const auto astStop{std::make_shared<AST_STOP>(fatherLoop, sourceLocation)};
  return astStop;
}

std::expected<std::shared_ptr<AST_IF>, Error>
Builder::createIf(const std::shared_ptr<AST> &condition,
         const std::shared_ptr<AST_BODY> &body,
         const std::vector<std::shared_ptr<AST_IF>> &elseIf,
         const std::shared_ptr<AST_BODY> &elseBody,
         const SourceLocation &sourceLocation) noexcept {
  const auto astIf{std::make_shared<AST_IF>(condition, body, elseIf, elseBody,
                                            sourceLocation)};
  condition->setFather(astIf);
  body->setFather(astIf);
  const std::vector<std::shared_ptr<AST_IF>> &elseIfs{astIf->elseIf()};
  for (const auto &elseIf_ : elseIfs) {
    elseIf_->setFather(astIf);
  }
  elseBody->setFather(astIf);
  return astIf;
}

std::expected<std::shared_ptr<AST_SWITCH>, Error>
Builder::createSwitch(const std::shared_ptr<AST> &condition,
             const std::vector<std::shared_ptr<AST_CASE>> &cases,
             const SourceLocation &sourceLocation) noexcept {
  const auto astSwitch{
      std::make_shared<AST_SWITCH>(condition, cases, sourceLocation)};
  condition->setFather(astSwitch);
  const std::vector<std::shared_ptr<AST_CASE>> &cases_{astSwitch->cases()};
  for (const auto &statement : cases_) {
    statement->setFather(astSwitch);
  }
  return astSwitch;
}

std::expected<std::shared_ptr<AST_CASE>, Error>
Builder::createCase(const std::shared_ptr<AST> &match,
           const std::shared_ptr<AST_BODY> &body,
           const SourceLocation &sourceLocation) noexcept {
  const auto astCase{std::make_shared<AST_CASE>(match, body, sourceLocation)};
  match->setFather(astCase);
  body->setFather(astCase);
  return astCase;
}

std::expected<std::shared_ptr<AST_TERNARY>, Error>
Builder::createTernary(const std::shared_ptr<AST> &condition,
              const std::shared_ptr<AST> &first,
              const std::shared_ptr<AST> &second,
              const SourceLocation &sourceLocation) noexcept {
  const auto astTernanry{
      std::make_shared<AST_TERNARY>(condition, first, second, sourceLocation)};
  condition->setFather(astTernanry);
  first->setFather(astTernanry);
  second->setFather(astTernanry);
  return astTernanry;
}

} // namespace nicole