#ifndef AST_SWITCH_H
#define AST_SWITCH_H

#include "ast_case.h"
#include "ast_default.h"
#include <memory>

namespace nicole {

class AST_SWITCH : public AST {
private:
  std::shared_ptr<AST> condition_;
  std::vector<std::shared_ptr<AST_CASE>> cases_;
  std::shared_ptr<AST_DEFAULT> default_;

public:
  explicit AST_SWITCH(const std::shared_ptr<AST> &condition,
                      const std::vector<std::shared_ptr<AST_CASE>> &cases,
                      const std::shared_ptr<AST_DEFAULT> &default__) noexcept
      : AST(AST_TYPE::SWITCH), condition_{condition}, cases_{cases},
        default_{default__} {}

  [[nodiscard]] const std::shared_ptr<AST> &condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST_CASE>> &
  cases() const noexcept {
    return cases_;
  }

  [[nodiscard]] const std::shared_ptr<AST_DEFAULT> &
  defaultCase() const noexcept {
    return default_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif