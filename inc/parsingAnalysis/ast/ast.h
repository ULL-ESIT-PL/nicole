#ifndef AST_H
#define AST_H

#include "../../lexicalAnalysis/sourceLocation.h"
#include "../../visitors/codeGeneration/codeGeneration.h"
#include "../../visitors/fillSemanticInfo/fillSemanticInfo.h"
#include "../../visitors/monomorphize/monomorphize.h"
#include "../../visitors/printTree/printTree.h"
#include "../../visitors/typeAnalysis/typeAnalysis.h"
#include "../../visitors/validateTree/validateTree.h"
#include "astType.h"
#include <memory>
#include <variant>

namespace nicole {

class AST {
private:
  long long unsigned nodeId_;
  AST_TYPE type_;
  std::weak_ptr<AST> father_;
  SourceLocation srcLoc_;
  mutable std::shared_ptr<Type> returnedFromTypeAnalysis_{nullptr};

public:
  explicit AST(const long long unsigned nodeId, const AST_TYPE type,
               const SourceLocation &srcLoc) noexcept
      : nodeId_{nodeId}, type_{type}, srcLoc_{srcLoc} {}

  virtual ~AST() noexcept = default;

  [[nodiscard]] long long unsigned nodeId() const noexcept { return nodeId_; }

  [[nodiscard]] AST_TYPE type() const noexcept { return type_; }

  [[nodiscard]] const std::shared_ptr<AST> father() const noexcept {
    return father_.lock();
  }

  [[nodiscard]] const SourceLocation &srcLoc() const noexcept {
    return srcLoc_;
  }

  [[nodiscard]] const std::shared_ptr<Type> &
  returnedFromTypeAnalysis() const noexcept {
    return returnedFromTypeAnalysis_;
  }

  void setFather(const std::shared_ptr<AST> &father) noexcept {
    father_ = father;
  }

  void
  setReturnedFromAnalysis(const std::shared_ptr<Type> &type) const noexcept {
    returnedFromTypeAnalysis_ = type;
  }

  [[nodiscard]] virtual std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept = 0;

  [[nodiscard]] virtual std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept = 0;

  [[nodiscard]] virtual std::expected<std::monostate, Error>
  accept(const FillSemanticInfo &visitor) const noexcept = 0;

  [[nodiscard]] virtual std::expected<std::shared_ptr<Type>, Error>
  accept(const TypeAnalysis &visitor) const noexcept = 0;

  [[nodiscard]] virtual std::expected<std::monostate, Error>
  accept(const Monomorphize &visitor) const noexcept = 0;

  [[nodiscard]] virtual std::expected<llvm::Value*, Error>
  accept(const CodeGeneration &visitor) const noexcept = 0;
};

} // namespace nicole

#endif // AST_H
