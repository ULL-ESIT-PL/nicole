#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include "userTypes/genericInstanceType.h"
#include "userTypes/userType.h"
#include "specialTypes/constType.h"
#include "specialTypes/nullType.h"
#include "specialTypes/voidType.h"
#include "specialTypes/ptrType.h"
#include "specialTypes/vectorType.h"
#include "basicTypes/basicTypes.h"
#include "userTypes/genericParameter.h"

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace nicole {

class TypeTable {
private:
  std::unordered_set<std::shared_ptr<Type>> types_{};

public:
   [[nodiscard]] bool hasType(const std::shared_ptr<Type>& type) const noexcept;

   void insertType(const std::shared_ptr<Type>& type) const noexcept;
};

} // namespace nicole

#endif