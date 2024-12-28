#ifndef AST_H
#define AST_H

#include "astType.h"
#include <memory>

namespace nicole {

class AST {
private:
  AST_TYPE type_;
  std::weak_ptr<AST> father_;

public:
  explicit AST(const AST_TYPE type,
               const std::shared_ptr<AST> &father = nullptr) noexcept
      : type_{type}, father_{father} {}

  virtual ~AST() noexcept = default;

  [[nodiscard]] AST_TYPE type() const noexcept { return type_; }

  [[nodiscard]] std::shared_ptr<AST> father() const noexcept { return father_.lock(); }
};

} // namespace nicole

#endif // AST_H
