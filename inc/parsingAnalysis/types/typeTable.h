#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include "boolType.h"
#include "charType.h"
#include "doubleType.h"
#include "floatType.h"
#include "genericType.h"
#include "intType.h"
#include "nullType.h"
#include "ptrType.h"
#include "strType.h"
#include "userType.h"
#include "vectorType.h"
#include "voidType.h"
#include <memory>
#include <unordered_map>

namespace nicole {

class TypeTable {
private:
  std::unordered_map<std::string, std::shared_ptr<GenericType>> types_{};

public:
   [[nodiscard]] bool hasType(std::string) const noexcept;
};

} // namespace nicole

#endif