#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include <iostream>

#include "../parsingAnalysis/ast/utils/convert.h"
#include "../parsingAnalysis/types/genericType.h"
#include "visitor.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"

namespace nicole {

// Visitor pattern to translate the AST to IR (Intermediate Representation)
// using LLVM library
class CodeGeneration final : public Visitor<llvm::Value *> {
private:
  llvm::LLVMContext *context_;
  llvm::Module *module_;
  mutable llvm::IRBuilder<> builder_;

public:
  CodeGeneration(llvm::LLVMContext *context, llvm::Module *module,
                 llvm::BasicBlock *entry)
      : context_{context}, module_{module}, builder_{entry} {}

  llvm::Value *visit(const NodeLiteralBool *node) const override;

  llvm::Value *visit(const NodeLiteralChar *node) const override;

  llvm::Value *visit(const NodeLiteralFloat *node) const override;

  llvm::Value *visit(const NodeLiteralDouble *node) const override;

  llvm::Value *visit(const NodeLiteralInt *node) const override;

  llvm::Value *visit(const NodeLiteralString *node) const override;

  llvm::Value *visit(const NodeReturn *node) const override;

  llvm::Value *visit(const NodeBinaryOp *node) const override;

  llvm::Value *visit(const NodeUnaryOp *node) const override;

  llvm::Value *visit(const NodeIncrement *node) const override;

  llvm::Value *visit(const NodeStatement *node) const override;

  llvm::Value *visit(const NodeStatementList *node) const override;

  llvm::Value *visit(const NodeVariableDeclaration *node) const override;

  llvm::Value *visit(const NodeConstDeclaration *node) const override;

  llvm::Value *visit(const NodeStructDeclaration *node) const override;

  llvm::Value *visit(const NodeFunctionDeclaration *node) const override;

  llvm::Value *visit(const NodeStructConstructor *node) const override;

  llvm::Value *visit(const NodeVariableCall *node) const override;

  llvm::Value *visit(const NodeFunctionCall *node) const override;

  llvm::Value *visit(const NodeVariableReassignment *node) const override;

  llvm::Value *visit(const NodeSelfReassignment *node) const override;

  llvm::Value *visit(const NodeSwitchStatement *node) const override;

  llvm::Value *visit(const NodeCaseStatement *node) const override;

  llvm::Value *visit(const NodeIfStatement *node) const override;

  llvm::Value *visit(const NodeWhileStatement *node) const override;

  llvm::Value *visit(const NodeDoWhileStatement *node) const override;

  llvm::Value *visit(const NodeForStatement *node) const override;

  llvm::Value *visit(const NodeStop *node) const override;

  llvm::Value *visit(const NodePass *node) const override;

  llvm::Value *visit(const NodePrint *node) const override;

  llvm::Value *visit(const NodeImport *node) const override;

  llvm::Value *visit(const Tree *tr) const override;

  llvm::Value *generate(const Tree *tr) const;
};

} // namespace nicole

#endif