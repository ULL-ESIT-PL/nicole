#ifndef AST_IMPORT_H
#define AST_IMPORT_H

#include "../ast.h"
#include <filesystem>
#include <memory>

namespace nicole {

class AST_IMPORT : public AST {
private:
  std::filesystem::path path_;

public:
  explicit AST_IMPORT(const std::filesystem::path &path) noexcept
      : AST(AST_TYPE::IMPORT), path_{path} {}

  [[nodiscard]] const std::filesystem::path &path() const noexcept {
    return path_;
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
};

} // namespace nicole

#endif