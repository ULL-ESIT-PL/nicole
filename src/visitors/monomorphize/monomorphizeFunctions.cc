#include "../../../inc/parsingAnalysis/ast/functions/ast_funcCall.h"
#include "../../../inc/parsingAnalysis/ast/functions/ast_funcDecl.h"
#include "../../../inc/parsingAnalysis/ast/functions/ast_return.h"
#include "../../../inc/visitors/monomorphize/monomorphize.h"
#include <memory>
#include <variant>

namespace nicole {

/*
Needs to be monomporhized
- Comprobar que tenga genericos
- si no hay genericos realizar copia

def foo<T>() {
  foo2<T>();
}

*/
std::expected<std::monostate, Error>
Monomorphize::visit(const AST_FUNC_CALL *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_FUNC_CALL");

  for (const auto& arg : node->parameters()) {
    const auto result{arg->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }

  for (const auto &replacement : node->replaceOfGenerics()) {
    if (typeTable_->isCompundPlaceHolder(replacement)) {
      return {};
    }
  }
  for (const auto &arg : node->parameters()) {
    if (typeTable_->isCompundPlaceHolder(arg->returnedFromTypeAnalysis())) {
      return {};
    }
  }
  currentCallReplacements_ = node->replaceOfGenerics();
  for (const auto& type : currentCallReplacements_) {
    std::cout << type->toString() << "\n";
  }
  const auto funcOriginalDecl{funcDeclReferences.at(node->id()).front()};
  auto copyFunDecl{std::make_shared<AST_FUNC_DECL>(*funcOriginalDecl)};

  std::vector<std::pair<std::string, std::shared_ptr<Type>>> newParams{};
  std::expected<std::shared_ptr<Type>, Error> substituteParam{nullptr};
  for (auto param : copyFunDecl->parameters()) {
    if (typeTable_->isCompundPlaceHolder(param.second)) {
      substituteParam = typeTable_->applyGenericReplacements(
          param.second, currentGenericList_, currentCallReplacements_);
      if (!substituteParam) {
        return createError(substituteParam.error());
      }
      param.second = *substituteParam;
    }
    newParams.push_back({param.first, param.second});
  }

  std::expected<std::shared_ptr<Type>, Error> substituteReturnType{nullptr};
  if (typeTable_->isCompundPlaceHolder(copyFunDecl->returnType())) {
    substituteReturnType = typeTable_->applyGenericReplacements(
        copyFunDecl->returnType(), currentGenericList_,
        currentCallReplacements_);
    if (!substituteReturnType) {
      return createError(substituteReturnType.error());
    }
    copyFunDecl->setReturnType(*substituteReturnType);
  }

  copyFunDecl->setParameters(Parameters{newParams});

  Function mono{
    copyFunDecl->id(), {}, copyFunDecl->parameters(),
    copyFunDecl->returnType(), copyFunDecl->body()
  };
  auto mname = nameManglingFunction(mono, currentCallReplacements_);
  if (!mname) return createError(mname.error());
  mono = Function{
    *mname, {}, copyFunDecl->parameters(),
    copyFunDecl->returnType(), copyFunDecl->body()
  };
  functionTable_->insert(mono);

  /*
  if (auto r = copyFunDecl->body()->accept(*this); !r)
    return createError(r.error());
  */
  const auto copyy{copyFunDecl->accept(*this)};
  if (!copyy) {
    return createError(copyy.error());
  }

  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_FUNC_DECL *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FUNC_DECL");
  if (!node->generics().size()) {
    return {};
  }
  currentGenericList_ = node->generics();
  currentCallReplacements_.clear();
  insideDeclWithGenerics = true;
  funcDeclReferences[node->id()].push_back(
      std::make_shared<AST_FUNC_DECL>(*node));
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  currentGenericList_.clear();
  currentCallReplacements_.clear();
  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_RETURN");
  }
  if (!node->expression()) {
    return {};
  }
  const auto result{node->expression()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

} // namespace nicole