#include "../../inc/parsingAnalysis/builder.h"

namespace nicole {

std::shared_ptr<AST_BOOL>
Builder::createBool(const bool value,
                    const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_BOOL>(value, sourceLocation);
}

std::shared_ptr<AST_CHAR>
Builder::createChar(const char value,
                    const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_CHAR>(value, sourceLocation);
}

std::shared_ptr<AST_DOUBLE>
Builder::createDouble(const double value,
                      const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_DOUBLE>(value, sourceLocation);
}

std::shared_ptr<AST_FLOAT>
Builder::createFloat(const float value,
                     const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_FLOAT>(value, sourceLocation);
}

std::shared_ptr<AST_INT>
Builder::createInt(const int value,
                   const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_INT>(value, sourceLocation);
}

std::shared_ptr<AST_NULL>
Builder::createNull(const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_NULL>(sourceLocation);
}

std::shared_ptr<AST_STRING>
Builder::createString(const std::string value,
                      const SourceLocation &sourceLocation) noexcept {
  return std::make_shared<AST_STRING>(value, sourceLocation);
}

std::shared_ptr<AST_ADD>
Builder::createAdd(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation) noexcept {
  const auto astAdd{std::make_shared<AST_ADD>(op, left, right, sourceLocation)};
  left->setFather(astAdd);
  right->setFather(astAdd);
  return astAdd;
}

std::shared_ptr<AST_SUB>
Builder::createSub(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation) noexcept {
  const auto astSub{std::make_shared<AST_SUB>(op, left, right, sourceLocation)};
  left->setFather(astSub);
  right->setFather(astSub);
  return astSub;
}

std::shared_ptr<AST_MULT>
Builder::createMult(const Token &op, const std::shared_ptr<AST> &left,
                    const std::shared_ptr<AST> &right,
                    const SourceLocation &sourceLocation) noexcept {
  const auto astMult{
      std::make_shared<AST_MULT>(op, left, right, sourceLocation)};
  left->setFather(astMult);
  right->setFather(astMult);
  return astMult;
}

std::shared_ptr<AST_DIV>
Builder::createDiv(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation) noexcept {
  const auto astDiv{std::make_shared<AST_DIV>(op, left, right, sourceLocation)};
  left->setFather(astDiv);
  right->setFather(astDiv);
  return astDiv;
}

std::shared_ptr<AST_MODULE>
Builder::createModule(const Token &op, const std::shared_ptr<AST> &left,
                      const std::shared_ptr<AST> &right,
                      const SourceLocation &sourceLocation) noexcept {
  const auto astModule{
      std::make_shared<AST_MODULE>(op, left, right, sourceLocation)};
  left->setFather(astModule);
  right->setFather(astModule);
  return astModule;
}

std::shared_ptr<AST_SMALLER>
Builder::createSmaller(const Token &op, const std::shared_ptr<AST> &left,
                       const std::shared_ptr<AST> &right,
                       const SourceLocation &sourceLocation) noexcept {
  const auto astSmaller{
      std::make_shared<AST_SMALLER>(op, left, right, sourceLocation)};
  left->setFather(astSmaller);
  right->setFather(astSmaller);
  return astSmaller;
}

std::shared_ptr<AST_SMALLER_EQUAL>
Builder::createSmallerEqual(const Token &op, const std::shared_ptr<AST> &left,
                            const std::shared_ptr<AST> &right,
                            const SourceLocation &sourceLocation) noexcept {
  const auto astSmallerEqual{
      std::make_shared<AST_SMALLER_EQUAL>(op, left, right, sourceLocation)};
  left->setFather(astSmallerEqual);
  right->setFather(astSmallerEqual);
  return astSmallerEqual;
}

std::shared_ptr<AST_BIGGER>
Builder::createBigger(const Token &op, const std::shared_ptr<AST> &left,
                      const std::shared_ptr<AST> &right,
                      const SourceLocation &sourceLocation) noexcept {
  const auto astBigger{
      std::make_shared<AST_BIGGER>(op, left, right, sourceLocation)};
  left->setFather(astBigger);
  right->setFather(astBigger);
  return astBigger;
}

std::shared_ptr<AST_BIGGER_EQUAL>
Builder::createBiggerEqual(const Token &op, const std::shared_ptr<AST> &left,
                           const std::shared_ptr<AST> &right,
                           const SourceLocation &sourceLocation) noexcept {
  const auto astBiggerEqual{
      std::make_shared<AST_BIGGER_EQUAL>(op, left, right, sourceLocation)};
  left->setFather(astBiggerEqual);
  right->setFather(astBiggerEqual);
  return astBiggerEqual;
}

std::shared_ptr<AST_EQUAL>
Builder::createEqual(const Token &op, const std::shared_ptr<AST> &left,
                     const std::shared_ptr<AST> &right,
                     const SourceLocation &sourceLocation) noexcept {
  const auto astEqual{
      std::make_shared<AST_EQUAL>(op, left, right, sourceLocation)};
  left->setFather(astEqual);
  right->setFather(astEqual);
  return astEqual;
}

std::shared_ptr<AST_NOT_EQUAL>
Builder::createNotEqual(const Token &op, const std::shared_ptr<AST> &left,
                        const std::shared_ptr<AST> &right,
                        const SourceLocation &sourceLocation) noexcept {
  const auto astNotEqual{
      std::make_shared<AST_NOT_EQUAL>(op, left, right, sourceLocation)};
  left->setFather(astNotEqual);
  right->setFather(astNotEqual);
  return astNotEqual;
}

std::shared_ptr<AST_OR>
Builder::createOr(const Token &op, const std::shared_ptr<AST> &left,
                  const std::shared_ptr<AST> &right,
                  const SourceLocation &sourceLocation) noexcept {
  const auto astOr{std::make_shared<AST_OR>(op, left, right, sourceLocation)};
  left->setFather(astOr);
  right->setFather(astOr);
  return astOr;
}

std::shared_ptr<AST_AND>
Builder::createAnd(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation) noexcept {
  const auto astAnd{std::make_shared<AST_AND>(op, left, right, sourceLocation)};
  left->setFather(astAnd);
  right->setFather(astAnd);
  return astAnd;
}

[[nodiscard]] static std::shared_ptr<AST_NOT>
createNot(const Token &op, const std::shared_ptr<AST> &value,
          const SourceLocation &sourceLocation) noexcept {
  const auto astNot{std::make_shared<AST_NOT>(op, value, sourceLocation)};
  value->setFather(astNot);
  return astNot;
}

[[nodiscard]] static std::shared_ptr<AST_INCREMENT>
createIncrement(const Token &op, const std::shared_ptr<AST> &value,
                const SourceLocation &sourceLocation) noexcept {
  const auto astIncrement{
      std::make_shared<AST_INCREMENT>(op, value, sourceLocation)};
  value->setFather(astIncrement);
  return astIncrement;
}

[[nodiscard]] static std::shared_ptr<AST_DECREMENT>
createDecrement(const Token &op, const std::shared_ptr<AST> &value,
                const SourceLocation &sourceLocation) noexcept {
  const auto astDecrement{
      std::make_shared<AST_DECREMENT>(op, value, sourceLocation)};
  value->setFather(astDecrement);
  return astDecrement;
}

[[nodiscard]] static std::shared_ptr<AST_PRINT>
createPrint(const std::shared_ptr<AST> &value,
            const SourceLocation &sourceLocation) noexcept {
  const auto astPrint{std::make_shared<AST_PRINT>(value, sourceLocation)};
  value->setFather(astPrint);
  return astPrint;
}

[[nodiscard]] static std::shared_ptr<AST_IMPORT>
createImport(const std::filesystem::path &path,
             const SourceLocation &sourceLocation) noexcept {
  const auto astImport{std::make_shared<AST_IMPORT>(path, sourceLocation)};
  return astImport;
}

[[nodiscard]] static std::shared_ptr<AST_STATEMENT>
createStatement(const std::shared_ptr<AST> &expression,
                const SourceLocation &sourceLocation) noexcept {
  const auto astStatement{
      std::make_shared<AST_STATEMENT>(expression, sourceLocation)};
  expression->setFather(astStatement);
  return astStatement;
}

[[nodiscard]] static std::shared_ptr<AST_BODY>
createBody(const std::vector<std::shared_ptr<AST_STATEMENT>> &body,
           const SourceLocation &sourceLocation) noexcept {
  const auto astBody{std::make_shared<AST_BODY>(body, sourceLocation)};
  const std::vector<std::shared_ptr<AST_STATEMENT>> &statements{
      astBody->body()};
  for (const auto &statement : statements) {
    statement->setFather(astBody);
  }
  return astBody;
}

[[nodiscard]] static std::shared_ptr<AST_COMMA>
createCOMMA(const std::vector<std::shared_ptr<AST_STATEMENT>> &body,
            const SourceLocation &sourceLocation) noexcept {
  const auto astComma{std::make_shared<AST_COMMA>(body, sourceLocation)};
  const std::vector<std::shared_ptr<AST_STATEMENT>> &statements{
      astComma->body()};
  for (const auto &statement : statements) {
    statement->setFather(astComma);
  }
  return astComma;
}

[[nodiscard]] static std::shared_ptr<AST_WHILE>
createWhile(const std::shared_ptr<AST> &condition,
            const std::shared_ptr<AST_BODY> &body,
            const SourceLocation &sourceLocation) noexcept {
  const auto astWhile{
      std::make_shared<AST_WHILE>(condition, body, sourceLocation)};
  condition->setFather(astWhile);
  body->setFather(astWhile);
  return astWhile;
}

[[nodiscard]] static std::shared_ptr<AST_FOR>
createFor(const std::shared_ptr<AST_COMMA> &init,
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

[[nodiscard]] static std::shared_ptr<AST_DO_WHILE>
createDoWhile(const std::shared_ptr<AST_BODY> &body,
              const std::shared_ptr<AST> &condition,
              const SourceLocation &sourceLocation) noexcept {
  const auto astDoWhile{
      std::make_shared<AST_DO_WHILE>(body, condition, sourceLocation)};
  condition->setFather(astDoWhile);
  body->setFather(astDoWhile);
  return astDoWhile;
}

[[nodiscard]] static std::shared_ptr<AST_PASS>
createPass(const std::shared_ptr<AST> &fatherLoop,
           const SourceLocation &sourceLocation) noexcept {
  const auto astPass{std::make_shared<AST_PASS>(fatherLoop, sourceLocation)};
  return astPass;
}

[[nodiscard]] static std::shared_ptr<AST_STOP>
createStop(const std::shared_ptr<AST> &fatherLoop,
           const SourceLocation &sourceLocation) noexcept {
  const auto astStop{std::make_shared<AST_STOP>(fatherLoop, sourceLocation)};
  return astStop;
}

[[nodiscard]] static std::shared_ptr<AST_IF>
createIf(const std::shared_ptr<AST> &condition,
         const std::shared_ptr<AST_BODY> &body,
         const std::vector<std::shared_ptr<AST_IF>> &elseIf,
         const std::shared_ptr<AST_BODY> &elseBody,
         const SourceLocation &sourceLocation) noexcept {
  const auto astIf{std::make_shared<AST_IF>(condition, body, elseIf, elseBody, sourceLocation)};
  condition->setFather(astIf);
  body->setFather(astIf);
  const std::vector<std::shared_ptr<AST_IF>> &elseIfs{
      astIf->elseIf()};
  for (const auto &elseIf_ : elseIfs) {
    elseIf_->setFather(astIf);
  }
  elseBody->setFather(astIf);
  return astIf;
}

[[nodiscard]] static std::shared_ptr<AST_SWITCH>
createSwitch(const std::shared_ptr<AST> &condition,
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

[[nodiscard]] static std::shared_ptr<AST_CASE>
createCase(const std::shared_ptr<AST> &match,
           const std::shared_ptr<AST_BODY> &body,
           const SourceLocation &sourceLocation) noexcept {
  const auto astCase{std::make_shared<AST_CASE>(match, body, sourceLocation)};
  match->setFather(astCase);
  body->setFather(astCase);
  return astCase;
}

[[nodiscard]] static std::shared_ptr<AST_TERNARY>
createTernary(const std::shared_ptr<AST> &condition,
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