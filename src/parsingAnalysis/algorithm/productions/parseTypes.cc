#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<Type>, Error>
TopDown::parseType() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<Type>, Error>
TopDown::parseTypeAtVarDecl(const bool isConst) const noexcept {
  if (isConst) {
  }
  return nullptr;
}

} // namespace nicole

// const name: type = value;
// let name: type = value;
// def fun<Generics>(id: type): type {}
// struct name: extends type<Generics> {}
// constructorDecl<Generics>(id: type) {}
// constructorCall<Generics>(){}
// funCall<Generics>()()