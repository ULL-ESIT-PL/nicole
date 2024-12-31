#ifndef AST_STOP_H
#define AST_STOP_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_STOP : public AST {
private:
  std::shared_ptr<AST> fatherLoop_;

public:
  explicit AST_STOP(const std::shared_ptr<AST> &fatherLoop,
                   const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::STOP, sourceLocation), fatherLoop_{fatherLoop} {}

  [[nodiscard]] const std::shared_ptr<AST> &fatherLoop() const noexcept {
    return fatherLoop_;
  }
};

} // namespace nicole

#endif