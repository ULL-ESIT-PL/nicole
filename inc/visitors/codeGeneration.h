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

  llvm::Value* visit(
      const std::unique_ptr<NodeLiteralInt>& node) const override;

  llvm::Value* visit(const std::unique_ptr<NodeBinary>& node) const override;

  llvm::Value* visit(const std::unique_ptr<NodeStatement>& node) const override;
};

}  // namespace nicole

#endif