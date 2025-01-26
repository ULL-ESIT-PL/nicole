#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_CHAINED>, Error>
TopDown::parseChainedExpression() const noexcept {
  std::shared_ptr<AST> basePtr{nullptr};
  std::vector<std::shared_ptr<AST>> operations{};
  const Token id{*tkStream_.current()};
  if (!tkStream_.eat()) {
    return std::unexpected{Error{ERROR_TYPE::SINTAX,
                                 "failed to eat " + tkStream_.current()->raw() +
                                     " at " + tkStream_.current()->locInfo()}};
  }

  // just a variable call
  if (tkStream_.current()->type() != TokenType::LP or
      tkStream_.current()->type() != TokenType::LC or
      tkStream_.current()->type() != TokenType::DOT) {
    const std::expected<std::shared_ptr<AST_VAR_CALL>, Error> varCall{
        Builder::createVarCall(id.raw())};
    basePtr = *varCall;
    return Builder::createChained(basePtr, operations);
  }
  
  return Builder::createChained(basePtr, operations);
}

} // namespace nicole