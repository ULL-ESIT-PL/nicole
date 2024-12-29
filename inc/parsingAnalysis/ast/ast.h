#ifndef AST_H
#define AST_H

#include "astType.h"
#include "sourceLocation.h"
#include <memory>

namespace nicole {

class AST : public std::enable_shared_from_this<AST> {
private:
  AST_TYPE type_;
  SourceLocation sourceLocation_;
  std::weak_ptr<AST> father_;

public:
  explicit AST(const AST_TYPE type,
                const SourceLocation& sourceLocation) noexcept
      : type_{type}, sourceLocation_{sourceLocation} {}

  virtual ~AST() noexcept = default;

  [[nodiscard]] AST_TYPE type() const noexcept { return type_; }

  [[nodiscard]] SourceLocation sourceLocation() const noexcept { return sourceLocation_; }

  [[nodiscard]] const std::shared_ptr<AST> father() const noexcept { return father_.lock(); }

  void setFather(const std::shared_ptr<AST>& father) noexcept { father_ = father; }
};

} // namespace nicole

#endif // AST_H
