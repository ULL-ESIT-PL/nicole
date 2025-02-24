#ifndef AST_STOP_H
#define AST_STOP_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_STOP : public AST {
private:
  std::shared_ptr<AST> fatherLoop_;

public:
  explicit AST_STOP(const std::shared_ptr<AST> &fatherLoop) noexcept
      : AST(AST_TYPE::STOP), fatherLoop_{fatherLoop} {}

  [[nodiscard]] const std::shared_ptr<AST> &fatherLoop() const noexcept {
    return fatherLoop_;
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