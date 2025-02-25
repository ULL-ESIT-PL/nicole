#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include <memory>
#include <string>
#include <unordered_map>
#include "../../parsingAnalysis/types/basicTypes/basicTypes.h"
#include "../../parsingAnalysis/types/userTypes/userType.h"
#include "../../parsingAnalysis/types/userTypes/genericInstanceType.h"
#include "../../parsingAnalysis/types/specialTypes/constType.h"
#include "../../parsingAnalysis/types/specialTypes/nullType.h"
#include "../../parsingAnalysis/types/specialTypes/ptrType.h"
#include "../../parsingAnalysis/types/specialTypes/vectorType.h"
#include "../../parsingAnalysis/types/specialTypes/voidType.h"
#include "../../parsingAnalysis/types/type.h"
#include "../../parsingAnalysis/types/type.h"
#include "../../parsingAnalysis/types/type.h"

namespace nicole {

class TypeTable {
private:
  std::unordered_map<std::string, std::shared_ptr<Type>> table_;

public:
  [[nodiscard]] bool has(const std::string & id) const noexcept;

  [[nodiscard]] const std::shared_ptr<Type> & getType(const std::string & id) const noexcept;

  void insert(const std::shared_ptr<Type> &type) noexcept;
};

} // namespace nicole

#endif