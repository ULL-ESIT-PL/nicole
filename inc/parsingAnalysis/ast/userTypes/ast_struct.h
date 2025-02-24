#ifndef AST_STRUCT_H
#define AST_STRUCT_H

#include "ast_constructorDecl.h"
#include "ast_destructorDecl.h"
#include "ast_methodDecl.h"
#include "attributes.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_STRUCT final : public AST {
private:
  std::string id_;
  std::vector<GenericParameter> generics_;
  std::unique_ptr<std::string> fatherType_;
  Attributes attributes_;
  std::vector<std::shared_ptr<AST_METHOD_DECL>> methods_;
  std::shared_ptr<AST_CONSTRUCTOR_DECL> constructor_;
  std::shared_ptr<AST_DESTRUCTOR_DECL> destructor_;

public:
  explicit AST_STRUCT(
      const std::string &id, const std::vector<GenericParameter> &generics,
      std::unique_ptr<std::string> fatherType, const Attributes &attributes,
      const std::vector<std::shared_ptr<AST_METHOD_DECL>> &methods,
      const std::shared_ptr<AST_CONSTRUCTOR_DECL> &constructor,
      const std::shared_ptr<AST_DESTRUCTOR_DECL> &destructor) noexcept
      : AST(AST_TYPE::STRUCT_DECL), id_{id}, generics_{generics},
        fatherType_{std::move(fatherType)}, attributes_{attributes},
        methods_{methods}, constructor_{constructor}, destructor_{destructor} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  [[nodiscard]] const std::unique_ptr<std::string> &
  fatherType() const noexcept {
    return fatherType_;
  }

  [[nodiscard]] const Attributes &attributes() const noexcept {
    return attributes_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST_METHOD_DECL>> &
  methods() const noexcept {
    return methods_;
  }

  [[nodiscard]] const std::shared_ptr<AST_CONSTRUCTOR_DECL> &
  constructor() const noexcept {
    return constructor_;
  }

  [[nodiscard]] const std::shared_ptr<AST_DESTRUCTOR_DECL> &
  destructor() const noexcept {
    return destructor_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::monostate, Error>
  accept(const FillScopes &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif