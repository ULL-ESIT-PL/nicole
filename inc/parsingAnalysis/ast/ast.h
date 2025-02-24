#ifndef AST_H
#define AST_H

#include "../../visitors/printTree.h"
#include "../../visitors/validateTree.h"
#include "astType.h"
#include <memory>

namespace nicole {

class AST {
private:
  AST_TYPE type_;
  std::weak_ptr<AST> father_;

public:
  explicit AST(const AST_TYPE type) noexcept : type_{type} {}

  virtual ~AST() noexcept = default;

  [[nodiscard]] AST_TYPE type() const noexcept { return type_; }

  [[nodiscard]] const std::shared_ptr<AST> father() const noexcept {
    return father_.lock();
  }

  void setFather(const std::shared_ptr<AST> &father) noexcept {
    father_ = father;
  }

  [[nodiscard]] virtual std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept = 0;

  [[nodiscard]] virtual std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept = 0;
};

} // namespace nicole

#endif // AST_H
