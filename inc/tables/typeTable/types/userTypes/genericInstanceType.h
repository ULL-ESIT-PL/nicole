#ifndef GENERIC_INSTANCE_TYPE_TYPE_H
#define GENERIC_INSTANCE_TYPE_TYPE_H

#include "userType.h"
#include <expected>
#include <memory>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace nicole {

class GenericInstanceType final : public UserType {
private:
  mutable std::vector<std::shared_ptr<Type>> typeArgs_;

public:
  GenericInstanceType(const std::shared_ptr<UserType> &genericType,
                      const std::vector<std::shared_ptr<Type>> &args) noexcept
      : UserType(*genericType),
        typeArgs_{args} {}

  [[nodiscard]] const std::vector<std::shared_ptr<Type>> &
  typeArgs() const noexcept {
    return typeArgs_;
  }

  [[nodiscard]] std::expected<std::monostate, Error>
  setGenericReplacement(const std::size_t pos,
                        const std::shared_ptr<Type> &type) const noexcept {
    if (pos >= typeArgs_.size()) {
      return createError(
          ERROR_TYPE::TYPE,
          "trying to access a invalid position in a replacement list");
    }
    typeArgs_[pos] = type;
    return {};
  }

  [[nodiscard]] std::string toString() const noexcept override {
    std::ostringstream oss;
    oss << UserType::toString() << "<";
    for (size_t i = 0; i < typeArgs_.size(); ++i) {
      oss << typeArgs_[i]->toString();
      if (i != typeArgs_.size() - 1)
        oss << ", ";
    }
    oss << ">";
    return oss.str();
  }

  [[nodiscard]] std::expected<llvm::Type*, Error>
  llvmVersion(llvm::LLVMContext &context) const noexcept override {
    // Recupera el struct opaco base por nombre
    llvm::StructType *origStruct = llvm::StructType::getTypeByName(context, name());
    if (!origStruct) {
      return createError(ERROR_TYPE::TYPE, "Unknown user type: " + name());
    }
    // Si el struct es opaco, define su cuerpo con los argumentos de tipo
    if (origStruct->isOpaque()) {
      std::vector<llvm::Type*> elements;
      elements.reserve(typeArgs_.size());
      for (const std::shared_ptr<nicole::Type> &arg : typeArgs_) {
        std::expected<llvm::Type *, Error> tyOrErr = arg->llvmVersion(context);
        if (!tyOrErr) {
          return createError(tyOrErr.error());
        }
        elements.push_back(*tyOrErr);
      }
      origStruct->setBody(elements, /*isPacked=*/false);
    }
    return origStruct;
  }
};

} // namespace nicole

#endif
