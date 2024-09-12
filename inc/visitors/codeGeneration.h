#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "visitor.h"

namespace nicole {
class CodeGeneration final : public Visitor {
 private:
  llvm::LLVMContext* context_;
  llvm::Module* module_;

 public:
  CodeGeneration(llvm::LLVMContext* context, llvm::Module* module)
      : context_{context}, module_{module} {}

  llvm::Value* visit(const NodeLiteralBool* node) const override;

  llvm::Value* visit(const NodeLiteralChar* node) const override;

  llvm::Value* visit(const NodeLiteralDouble* node) const override;

  llvm::Value* visit(const NodeLiteralInt* node) const override;

  llvm::Value* visit(const NodeLiteralString* node) const override;

  llvm::Value* visit(const NodeBinaryOp* node) const override;

  llvm::Value* visit(const NodeStatement* node) const override;

  llvm::Value* visit(const NodeStatementList* node) const override;
};

}  // namespace nicole

#endif