#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include "../../errors.h"
#include "types/basicTypes/basicTypes.h"
#include "types/specialTypes/constType.h"
#include "types/specialTypes/nullType.h"
#include "types/specialTypes/ptrType.h"
#include "types/specialTypes/vectorType.h"
#include "types/specialTypes/voidType.h"
#include "types/type.h"
#include "types/userTypes/genericInstanceType.h"
#include "types/userTypes/userType.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace nicole {

class TypeTable {
private:
  std::unordered_map<std::string, std::shared_ptr<Type>> table_{
      {"bool", std::make_shared<BasicType>(BasicKind::Bool)},
      {"int", std::make_shared<BasicType>(BasicKind::Int)},
      {"float", std::make_shared<BasicType>(BasicKind::Float)},
      {"double", std::make_shared<BasicType>(BasicKind::Double)},
      {"char", std::make_shared<BasicType>(BasicKind::Char)},
      {"str", std::make_shared<BasicType>(BasicKind::Str)},
      {"void", std::make_shared<VoidType>()},
  };

public:
  [[nodiscard]] bool has(const std::string &id) const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<Type>, Error>
  getType(const std::string &id) const noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  insert(const std::shared_ptr<Type> &type) noexcept;
};

} // namespace nicole

#endif