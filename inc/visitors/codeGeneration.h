#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "visitor.h"
#include <iostream>

namespace nicole {
class CodeGeneration final : public Visitor {
 private:
  llvm::LLVMContext* context_;
  llvm::Module* module_;
  llvm::BasicBlock* entry_;

 public:
  CodeGeneration(llvm::LLVMContext* context, llvm::Module* module, llvm::BasicBlock* entry)
      : context_{context}, module_{module}, entry_{entry} {}

  llvm::Value* visit(const NodeLiteralBool* node) const override;

  llvm::Value* visit(const NodeLiteralChar* node) const override;

  llvm::Value* visit(const NodeLiteralDouble* node) const override;

  llvm::Value* visit(const NodeLiteralInt* node) const override;

  llvm::Value* visit(const NodeLiteralString* node) const override;

  llvm::Value* visit(const NodeBinaryOp* node) const override;

  llvm::Value* visit(const NodeStatement* node) const override;

  llvm::Value* visit(const NodeStatementList* node) const override;

  llvm::Value* visit(const NodeVariableDeclaration* node) const override;

  llvm::Value* visit(const Tree* tr) const override;
};

}  // namespace nicole

#endif