#ifndef TOP_DOWN_H
#define TOP_DOWN_H

#include "parser.h"

namespace nicole {

// Top down recursive parser that can be used with any type of sintax, in this
// case we implement our productions with the Nicole language. In the futute I
// would like to extend it to let the user create its own productions outside
// the parser
class TopDown final : public Parser {
private:
  mutable std::shared_ptr<NodeStatementList> root_{nullptr};

  std::shared_ptr<NodeStatementList> parseStart() const;

  std::shared_ptr<NodeStatementList>
  parseBody(std::shared_ptr<VariableTable> &bodyScope,
            std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeStatementList>
  parseComma(std::shared_ptr<VariableTable> &bodyScope,
             std::shared_ptr<Node> father) const;

  // this one is used in definitions
  std::shared_ptr<ParamsDeclaration>
  parseParams(std::shared_ptr<VariableTable> &bodyScope,
              std::shared_ptr<Node> father) const;

  // this one is usde in calls
  std::vector<std::shared_ptr<Node>>
  parseParamsCall(std::shared_ptr<VariableTable> &bodyScope,
              std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeStatement>
  parseStatement(std::shared_ptr<VariableTable> currentScope,
                 std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeSwitchStatement>
  parseSwitchStatement(std::shared_ptr<VariableTable> currentScope,
                   std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeCaseStatement>
  parseCaseStatement(std::shared_ptr<VariableTable> currentScope,
                   std::shared_ptr<Node> father) const;
  
  std::shared_ptr<NodeCaseStatement>
  parseDefault(std::shared_ptr<VariableTable> currentScope,
                   std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeIfStatement>
  parseIfStatement(std::shared_ptr<VariableTable> currentScope,
                   std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeWhileStatement>
  parseWhileStatement(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeDoWhileStatement>
  parseDoWhileStatement(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeForStatement>
  parseForStatement(std::shared_ptr<VariableTable> currentScope,
                    std::shared_ptr<Node> father) const;

  std::shared_ptr<NodePrint>
  parsePrintStatement(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeStructDeclaration>
  parseStructDeclaration(std::shared_ptr<VariableTable> currentScope,
                         std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeFunctionDeclaration>
  parseFunctionDeclaration(std::shared_ptr<VariableTable> currentScope,
                           std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeReturn>
  parseReturn(std::shared_ptr<VariableTable> &currentScope,
              std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeImport>
  parseImport(std::shared_ptr<VariableTable> &currentScope,
              std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeSelfReassignment>
  parseSelfAssignment(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const;

  std::shared_ptr<Node>
  parseVarDeclaration(std::shared_ptr<VariableTable> currentScope,
                      std::shared_ptr<Node> father) const;

  std::shared_ptr<Node>
  parseLogicalOr(std::shared_ptr<VariableTable> currentScope,
                 std::shared_ptr<Node> father) const;

  std::shared_ptr<Node>
  parseLogicalAnd(std::shared_ptr<VariableTable> currentScope,
                  std::shared_ptr<Node> father) const;

  // equal not
  std::shared_ptr<Node>
  parseLogicalEqual(std::shared_ptr<VariableTable> currentScope,
                    std::shared_ptr<Node> father) const;

  // < <=...
  std::shared_ptr<Node>
  parseCompare(std::shared_ptr<VariableTable> currentScope,
               std::shared_ptr<Node> father) const;

  std::shared_ptr<Node>
  parseAdd_Sub(std::shared_ptr<VariableTable> currentScope,
               std::shared_ptr<Node> father) const;

  std::shared_ptr<Node>
  parseMult_Div(std::shared_ptr<VariableTable> currentScope,
                std::shared_ptr<Node> father) const;

  std::shared_ptr<Node> parseFactor(std::shared_ptr<VariableTable> currentScope,
                                    std::shared_ptr<Node> father) const;

  std::shared_ptr<NodeFunctionCall>
  parseFunctionCall(const std::string &id,
                    std::shared_ptr<VariableTable> currentScope,
                    std::shared_ptr<Node> father) const;

public:
  TopDown(std::shared_ptr<Sintax> sintax) : Parser{sintax} {}

  ~TopDown() = default;

  std::shared_ptr<Tree> parse(const std::filesystem::path &path) const override;
};

} // namespace nicole

#endif