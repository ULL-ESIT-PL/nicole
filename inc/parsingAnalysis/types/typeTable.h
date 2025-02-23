#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include "basicTypes/basicTypes.h"
#include "specialTypes/constType.h"
#include "specialTypes/nullType.h"
#include "specialTypes/ptrType.h"
#include "specialTypes/vectorType.h"
#include "specialTypes/voidType.h"
#include "userTypes/genericInstanceType.h"
#include "userTypes/genericParameter.h"
#include "userTypes/userType.h"

#include <memory>
#include <unordered_map>

namespace nicole {

class TypeTable {
private:
  std::unordered_map<std::string, std::shared_ptr<Type>> table_;

public:
  explicit TypeTable() noexcept;

  [[nodiscard]] std::shared_ptr<Type>
  internType(const std::string &key,
             const std::shared_ptr<Type> &type) noexcept;

  // === Métodos GET: solo recuperan si existe ===

  [[nodiscard]] std::shared_ptr<Type>
  getType(const std::string &key) const noexcept;

  [[nodiscard]] std::shared_ptr<Type> getGenericInstance(
      const std::string &templateName,
      const std::vector<std::shared_ptr<Type>> &typeArgs) const noexcept;

  [[nodiscard]] std::shared_ptr<Type>
  getPointerType(const std::shared_ptr<Type> &baseType) const noexcept;

  [[nodiscard]] std::shared_ptr<Type>
  getVectorType(const std::shared_ptr<Type> &elementType) const noexcept;

  // === Métodos CREATE: crean e internan si no existe ===

  [[nodiscard]] std::shared_ptr<Type> createGenericInstance(
      const std::string &templateName,
      const std::vector<std::shared_ptr<Type>> &typeArgs) noexcept;

  [[nodiscard]] std::shared_ptr<Type>
  createPointerType(const std::shared_ptr<Type> &baseType) noexcept;

  [[nodiscard]] std::shared_ptr<Type>
  createVectorType(const std::shared_ptr<Type> &elementType) noexcept;
};

} // namespace nicole

#endif