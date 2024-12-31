#ifndef AST_FOR_H
#define AST_FOR_H

#include "../statements/ast_body.h"
#include "../statements/ast_comma.h"
#include <memory>

namespace nicole {

class AST_FOR : public AST {
private:
  std::shared_ptr<AST_COMMA> init_;
  std::shared_ptr<AST> condition_;
  std::shared_ptr<AST_COMMA> update_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_FOR(const std::shared_ptr<AST_COMMA> &init,
                   const std::shared_ptr<AST> &condition,
                   const std::shared_ptr<AST_COMMA> &update,
                   const std::shared_ptr<AST_BODY> &body,
                   const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::FOR, sourceLocation), init_{init}, condition_{condition},
        update_{update}, body_{body} {}

  [[nodiscard]] const std::shared_ptr<AST_COMMA> &init() const noexcept {
    return init_;
  }

  [[nodiscard]] const std::shared_ptr<AST> &condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::shared_ptr<AST_COMMA> &update() const noexcept {
    return update_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif