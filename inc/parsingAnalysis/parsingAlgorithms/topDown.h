#ifndef TOP_DOWN_H
#define TOP_DOWN_H

#include "parser.h"

namespace nicole {

class TopDown final : public Parser {
 private:
  mutable std::unique_ptr<NodeStatementList> root_{nullptr};

  std::unique_ptr<NodeStatementList> parseStart() const;

  std::unique_ptr<NodeStatementList> parseBody(
      std::shared_ptr<VariableTable>& bodyScope) const;

  std::unique_ptr<NodeStatement> parseStatement(
      std::shared_ptr<VariableTable> currentScope) const;

  std::unique_ptr<NodeIfStatement> parseIfStatement(
      std::shared_ptr<VariableTable> currentScope) const;

  std::unique_ptr<Node> parseVarDeclaration(
      std::shared_ptr<VariableTable> currentScope) const;

  std::unique_ptr<Node> parseAdd_Sub(
      std::shared_ptr<VariableTable> currentScope) const;

  std::unique_ptr<Node> parseFactor(
      std::shared_ptr<VariableTable> currentScope) const;

 public:
  TopDown(std::unique_ptr<Sintax> sintax) : Parser{std::move(sintax)} {}

  ~TopDown() = default;

  std::unique_ptr<Tree> parse(const std::filesystem::path& path) const override;
};

}  // namespace nicole

#endif