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
   [[nodiscard]] bool hasType(const std::shared_ptr<Type>& type) const noexcept;

   void insertType(const std::shared_ptr<Type>& type) const noexcept;
};

} // namespace nicole

#endif