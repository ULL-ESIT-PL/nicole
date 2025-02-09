#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <memory>
#include <vector>

namespace nicole {

const std::expected<std::shared_ptr<AST_WHILE>, Error>
TopDown::parseWhile() const noexcept {
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  const std::expected<std::shared_ptr<AST_CONDITION>, Error> condition{
      parseCondition(false)};
  if (!condition || !*condition) {
    return createError(condition ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : condition.error());
  }
  const std::expected<std::shared_ptr<AST_BODY>, Error> body{parseBody()};
  if (!body || !*body) {
    return createError(body ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                            : body.error());
  }
  return Builder::createWhile(*condition, *body);
}

const std::expected<std::shared_ptr<AST_FOR>, Error>
TopDown::parseFor() const noexcept {
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  if (tkStream_.current()->type() != TokenType::LP) {
    return createError(ERROR_TYPE::SINTAX,
                       "missing left parenthesis of for at " +
                           tkStream_.current()->locInfo());
  }
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  if (tkStream_.current()->type() == TokenType::RP) {
    return createError(ERROR_TYPE::SINTAX,
                       "empty for at " + tkStream_.current()->locInfo());
  }
  std::vector<std::shared_ptr<AST>> init{};
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::SEMICOLON) {
    const std::expected<std::shared_ptr<AST>, Error> expression{
        parseVarDecl(true)};
    if (!expression || !*expression) {
      return createError(expression
                             ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                             : expression.error());
    }
    init.push_back(*expression);
    if (tkStream_.current()->type() == TokenType::COMMA) {
      if (!tkStream_.eat()) {
        return createError(ERROR_TYPE::SINTAX,
                           "failed to eat " + tkStream_.current()->raw() +
                               " at " + tkStream_.current()->locInfo());
      }
      continue;
    } else if (tkStream_.current()->type() != TokenType::SEMICOLON) {
      return createError(ERROR_TYPE::SINTAX,
                         "missing comma or ; of FOR at " +
                             tkStream_.current()->locInfo());
    }
    break;
  }
  if (tkStream_.current()->type() != TokenType::SEMICOLON) {
    return createError(ERROR_TYPE::SINTAX, "missing ; after init of for at " +
                                               tkStream_.current()->locInfo());
  }
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }

  const std::expected<std::shared_ptr<AST_CONDITION>, Error> condition{
      (tkStream_.current()->type() == TokenType::SEMICOLON)
          ? *Builder::createCondition(*Builder::createBool(true))
          : parseCondition(true)};
  if (!condition || !*condition) {
    return createError(condition ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : condition.error());
  }
  if (tkStream_.current()->type() != TokenType::SEMICOLON) {
    return createError(ERROR_TYPE::SINTAX,
                       "missing ; after condition of for at " +
                           tkStream_.current()->locInfo());
  }
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  std::vector<std::shared_ptr<AST>> update{};
  while (tkStream_.currentPos() < tkStream_.size() and
         tkStream_.current()->type() != TokenType::RP) {
    const std::expected<std::shared_ptr<AST>, Error> expression{
        parseSelfAssignment(true)};
    if (!expression || !*expression) {
      return createError(expression
                             ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                             : expression.error());
    }
    update.push_back(*expression);
    if (tkStream_.current()->type() == TokenType::COMMA) {
      if (!tkStream_.eat()) {
        return createError(ERROR_TYPE::SINTAX,
                           "failed to eat " + tkStream_.current()->raw() +
                               " at " + tkStream_.current()->locInfo());
      }
      continue;
    } else if (tkStream_.current()->type() != TokenType::RP) {
      return createError(ERROR_TYPE::SINTAX,
                         "missing comma or ) of for at " +
                             tkStream_.current()->locInfo());
    }
    break;
  }
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  const std::expected<std::shared_ptr<AST_BODY>, Error> body{parseBody()};
  if (!body || !*body) {
    return createError(body ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                            : body.error());
  }
  return Builder::createFor(init, *condition, update, *body);
}

const std::expected<std::shared_ptr<AST_DO_WHILE>, Error>
TopDown::parseDoWhile() const noexcept {
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  const std::expected<std::shared_ptr<AST_BODY>, Error> body{parseBody()};
  if (!body || !*body) {
    return createError(body ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                            : body.error());
  }
  if (tkStream_.current()->type() != TokenType::WHILE) {
    return createError(ERROR_TYPE::SINTAX,
                       "missing while keyword of do while at " +
                           tkStream_.current()->locInfo());
  }
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  const std::expected<std::shared_ptr<AST_CONDITION>, Error> condition{
      parseCondition(false)};
  if (!condition || !*condition) {
    return createError(condition ? Error{ERROR_TYPE::NULL_NODE, "node is null"}
                                 : condition.error());
  }
  if (!tkStream_.current() or
      tkStream_.current()->type() != TokenType::SEMICOLON) {
    return createError(ERROR_TYPE::SINTAX,
                       "missing ; of do while statement at " +
                           (*tkStream_.lastRead()).locInfo());
  }
  return Builder::createDoWhile(*body, *condition);
}

const std::expected<std::shared_ptr<AST_PASS>, Error>
TopDown::parsePass() const noexcept {
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  if (tkStream_.current()->type() != TokenType::SEMICOLON) {
    return createError(ERROR_TYPE::SINTAX, "missing ; of pass statement at " +
                                               tkStream_.current()->locInfo());
  }
  return Builder::createPass(nullptr);
}

const std::expected<std::shared_ptr<AST_STOP>, Error>
TopDown::parseStop() const noexcept {
  if (!tkStream_.eat()) {
    return createError(ERROR_TYPE::SINTAX,
                       "failed to eat " + tkStream_.current()->raw() + " at " +
                           tkStream_.current()->locInfo());
  }
  if (tkStream_.current()->type() != TokenType::SEMICOLON) {
    return createError(ERROR_TYPE::SINTAX, "missing ; of stop statement at " +
                                               tkStream_.current()->locInfo());
  }
  return Builder::createStop(nullptr);
}

} // namespace nicole