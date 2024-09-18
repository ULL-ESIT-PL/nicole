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
  llvm::BasicBlock *entry_;

public:
  CodeGeneration(llvm::LLVMContext *context, llvm::Module *module,
                 llvm::BasicBlock *entry)
      : context_{context}, module_{module}, entry_{entry} {}

  llvm::Value *visit(const NodeLiteralBool *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeLiteralChar *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeLiteralDouble *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeLiteralInt *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeLiteralString *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeBinaryOp *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeStatement *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeStatementList *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeVariableDeclaration *node,
                     llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeVariableCall *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeVariableReassignment *node,
                     llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const NodeIfStatement *node, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;

  llvm::Value *visit(const Tree *tr, llvm::BasicBlock *currentEntry,
                     llvm::Module *currentModule) const override;
};

} // namespace nicole

#endif