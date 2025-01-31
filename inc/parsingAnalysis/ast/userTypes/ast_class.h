#ifndef AST_CLASS_H
#define AST_CLASS_H

#include "../ast.h"
#include "../functions/ast_funcDecl.h"
#include "attributes.h"
#include <vector>

namespace nicole {

class AST_CLASS final : public AST {
private:
  std::string id_;
  std::unique_ptr<std::string> fatherType_;
  Attributes attributes_;
  std::vector<std::shared_ptr<AST_FUNC_DECL>> methods_;
  std::shared_ptr<AST_FUNC_DECL> constructor_;
  std::shared_ptr<AST_FUNC_DECL> destructor_;

public:
  explicit AST_CLASS(const std::string &id,
                     std::unique_ptr<std::string> fatherType,
                     const Attributes &attributes,
                     const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods,
                     const std::shared_ptr<AST_FUNC_DECL> &constructor,
                     const std::shared_ptr<AST_FUNC_DECL> &destructor) noexcept
      : AST(AST_TYPE::CLASS_DECL), id_{id}, fatherType_{std::move(fatherType)},
        attributes_{attributes}, methods_{methods}, constructor_{constructor},
        destructor_{destructor} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::unique_ptr<std::string> &
  fatherType() const noexcept {
    return fatherType_;
  }

  [[nodiscard]] const Attributes &attributes() const noexcept {
    return attributes_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST_FUNC_DECL>> &
  methods() const noexcept {
    return methods_;
  }

  [[nodiscard]] const std::shared_ptr<AST_FUNC_DECL> &
  constructor() const noexcept {
    return constructor_;
  }

  [[nodiscard]] const std::shared_ptr<AST_FUNC_DECL> &
  destructor() const noexcept {
    return destructor_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif