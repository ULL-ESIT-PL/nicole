#ifndef AST_CONSTRUCTOR_CALL_H
#define AST_CONSTRUCTOR_CALL_H

#include "../../../tables/typeTable/types/type.h"
#include "../ast.h"
#include <string>
#include <vector>

namespace nicole {

class AST_CONSTRUCTOR_CALL final : public AST {
private:
  std::string id_;
  std::vector<std::shared_ptr<Type>> replaceOfGenerics_;
  std::vector<std::shared_ptr<AST>> parameters_;

public:
  explicit AST_CONSTRUCTOR_CALL(
      const long long unsigned nodeId, const std::string &id,
      const std::vector<std::shared_ptr<Type>> &replaceOfGenerics,
      const std::vector<std::shared_ptr<AST>> &parameters) noexcept
      : AST(nodeId, AST_TYPE::CONSTRUCTOR_CALL), id_{id},
        replaceOfGenerics_{replaceOfGenerics}, parameters_{parameters} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::vector<std::shared_ptr<Type>> &
  replaceOfGenerics() const noexcept {
    return replaceOfGenerics_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &
  parameters() const noexcept {
    return parameters_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::monostate, Error>
  accept(const FillSemanticInfo &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::shared_ptr<Type>, Error>
  accept(const TypeAnalysis &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::monostate, Error>
  accept(const Monomorphize &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::shared_ptr<llvm::Value>, Error>
  accept(const CodeGeneration &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif