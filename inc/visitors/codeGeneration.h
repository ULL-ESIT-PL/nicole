#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include <iostream>

#include "visitor.h"
#include "llvm/IR/IRBuilder.h"

namespace nicole {

class CodeGeneration final : public Visitor {
private:
  llvm::LLVMContext *context_;
  llvm::Module *module_;
  mutable llvm::IRBuilder<> builder_;

  llvm::Value *visit(const NodeLiteralBool *node) const override;

  llvm::Value *visit(const NodeLiteralChar *node) const override;

  llvm::Value *visit(const NodeLiteralDouble *node) const override;

  llvm::Value *visit(const NodeLiteralInt *node) const override;

  llvm::Value *visit(const NodeLiteralString *node) const override;

  llvm::Value *visit(const NodeBinaryOp *node) const override;

  llvm::Value *visit(const NodeStatement *node) const override;

  llvm::Value *visit(const NodeStatementList *node) const override;

  llvm::Value *visit(const NodeVariableDeclaration *node) const override;

  llvm::Value *visit(const NodeConstDeclaration *node) const override;

  llvm::Value *visit(const NodeVariableCall *node) const override;

  llvm::Value *visit(const NodeVariableReassignment *node) const override;

  llvm::Value *visit(const NodeIfStatement *node) const override;

  llvm::Value *visit(const Tree *tr) const override;

public:
  CodeGeneration(llvm::LLVMContext *context, llvm::Module *module,
                 llvm::BasicBlock *entry)
      : context_{context}, module_{module}, builder_{entry} {}

  llvm::Value *generate(const Tree *tr) const;
};

} // namespace nicole

#endif