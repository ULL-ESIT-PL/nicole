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
  Attributes attributes_;
  std::vector<std::shared_ptr<AST_FUNC_DECL>> methods_;
  std::shared_ptr<AST_FUNC_DECL> constructor_;
  std::shared_ptr<AST_FUNC_DECL> destructor_;
  std::shared_ptr<AST_FUNC_DECL> addOverloading_;

public:
  explicit AST_CLASS(const std::string &id, const Attributes &attributes,
                     const std::vector<std::shared_ptr<AST_FUNC_DECL>> &methods,
                     const std::shared_ptr<AST_FUNC_DECL> &constructor,
                     const std::shared_ptr<AST_FUNC_DECL> &destructor,
                     const std::shared_ptr<AST_FUNC_DECL> &addOverloading,
                     const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::CLASS_DECL, sourceLocation), id_{id},
        attributes_{attributes}, methods_{methods}, constructor_{constructor},
        destructor_{destructor}, addOverloading_{addOverloading} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

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

  [[nodiscard]] const std::shared_ptr<AST_FUNC_DECL> &
  addOverloading() const noexcept {
    return addOverloading_;
  }
};

} // namespace nicole

#endif