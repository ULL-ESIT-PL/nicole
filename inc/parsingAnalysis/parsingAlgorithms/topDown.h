#ifndef TOP_DOWN_H
#define TOP_DOWN_H

#include "parser.h"

namespace nicole {
class TopDown final : public Parser {
 private:
  llvm::LLVMContext* context_;  // momentaneo
  llvm::Module* module_;
 mutable std::unique_ptr<NodeStatement> root_{nullptr};

  std::unique_ptr<Node> parseStart() const;
  std::unique_ptr<Node> parseFactor() const;

 public:
  TopDown(llvm::LLVMContext* context, llvm::Module* module)
      : context_{context}, module_{module} {}

  ~TopDown() = default;

  std::unique_ptr<Node> parse(const std::filesystem::path& path) const override;
};
}  // namespace nicole

#endif