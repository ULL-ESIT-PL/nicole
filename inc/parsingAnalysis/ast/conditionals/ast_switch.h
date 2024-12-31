#ifndef AST_SWITCH_H
#define AST_SWITCH_H

#include "ast_case.h"
#include <memory>

namespace nicole {

class AST_SWITCH : public AST {
private:
  std::shared_ptr<AST> condition_;
  std::vector<std::shared_ptr<AST_CASE>> cases_;

public:
  explicit AST_SWITCH(const std::shared_ptr<AST> &condition,
                      const std::vector<std::shared_ptr<AST_CASE>> &cases,
                      const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::SWITCH, sourceLocation), condition_{condition},
        cases_{cases} {}

  [[nodiscard]] const std::shared_ptr<AST> &condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST_CASE>> &
  cases() const noexcept {
    return cases_;
  }
};

} // namespace nicole

#endif