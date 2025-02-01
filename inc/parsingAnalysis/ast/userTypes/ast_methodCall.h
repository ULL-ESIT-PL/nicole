#ifndef AST_METHOD_CALL_H
#define AST_METHOD_CALL_H

#include "../ast.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_METHOD_CALL : public AST {
private:
  std::string id_;
  std::vector<std::shared_ptr<AST>> parameters_;

public:
  explicit AST_METHOD_CALL(
      const std::string &id,
      const std::vector<std::shared_ptr<AST>> &parameters) noexcept
      : AST(AST_TYPE::METHOD_CALL), id_{id}, parameters_{parameters} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

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
};

} // namespace nicole

#endif