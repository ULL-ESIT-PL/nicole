#ifndef AST_H
#define AST_H

#include "astType.h"
#include "sourceLocation.h"
#include <memory>

namespace nicole {

class AST {
private:
  AST_TYPE type_;
  SourceLocation sourceLocation_;
  std::weak_ptr<AST> father_;

public:
  explicit AST(const AST_TYPE type,
                const SourceLocation& sourceLocation,
               const std::shared_ptr<AST> &father = nullptr) noexcept
      : type_{type}, sourceLocation_{sourceLocation}, father_{father} {}

  virtual ~AST() noexcept = default;

  [[nodiscard]] AST_TYPE type() const noexcept { return type_; }

  [[nodiscard]] SourceLocation sourceLocation() const noexcept { return sourceLocation_; }

  [[nodiscard]] std::shared_ptr<AST> father() const noexcept { return father_.lock(); }
};

} // namespace nicole

#endif // AST_H
