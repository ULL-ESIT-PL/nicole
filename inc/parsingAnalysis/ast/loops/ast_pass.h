#ifndef AST_PASS_H
#define AST_PASS_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_PASS : public AST {
private:
  std::shared_ptr<AST> fatherLoop_;

public:
  explicit AST_PASS(const std::shared_ptr<AST> &fatherLoop,
                   const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::PASS, sourceLocation), fatherLoop_{fatherLoop} {}

  [[nodiscard]] const std::shared_ptr<AST> &fatherLoop() const noexcept {
    return fatherLoop_;
  }
};

} // namespace nicole

#endif