#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include "type.h"

#include <memory>
#include <unordered_map>

namespace nicole {

class TypeTable {
private:
  std::unordered_map<std::string, std::shared_ptr<Type>> types_{};

public:
   [[nodiscard]] bool hasType(std::string) const noexcept;
};

} // namespace nicole

#endif