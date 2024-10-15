#ifndef NODE_IMPORT_H
#define NODE_IMPORT_H

#include <filesystem>
#include <memory>

#include "../node.h"

namespace nicole {

// Represents the equivalent of #include
class NodeImport final : public Node {
private:
  std::filesystem::path fileName_{};

public:
  NodeImport(const std::filesystem::path &fileName,
            std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::IMPORT, father}, fileName_{fileName} {};

  ~NodeImport() = default;

  std::filesystem::path fileName() const { return fileName_; }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif