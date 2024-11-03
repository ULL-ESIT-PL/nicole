#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "../ast/declaration/paramsDeclaration.h"
#include "../ast/declaration/functionTable.h"
#include "genericType.h"
#include "llvm/Support/ErrorHandling.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

#include <memory>
#include <string>
#include <vector>

namespace nicole {

// Used in structs and classes
class UserType : public GenericType {
private:
  std::shared_ptr<ParamsDeclaration> attributes_{nullptr};
  std::shared_ptr<FunctionTable> methods_{nullptr};

public:
  UserType(const std::string &name,
           std::shared_ptr<GenericType> father = nullptr)
      : GenericType{name, father} {};

  ~UserType() = default;

  llvm::Type *type(llvm::LLVMContext *context) const override {
    return llvm::StructType::getTypeByName(*context, name_);
  };

  std::shared_ptr<ParamsDeclaration> attributes() const { return attributes_; }

  void setAttributes(std::shared_ptr<ParamsDeclaration> attributes) {
    attributes_ = attributes;
  };

  std::tuple<size_t, std::string, std::string> attribute(const std::string &atribute) const {
    size_t index{0};
    bool found{false};
    const auto attr{attributes_->paramters()};
    std::pair<std::string, std::string> par;
    for (size_t i{0}; i < attr.size(); ++i) {
      if (attr[i].first == atribute) {
        found = true;
        index = i;
        par = attr[i];
      }
    }
    if (!found) {
      llvm::report_fatal_error("Attribute not found");
    }
    return {index, atribute, par.second};
  };
};

} // namespace nicole

#endif