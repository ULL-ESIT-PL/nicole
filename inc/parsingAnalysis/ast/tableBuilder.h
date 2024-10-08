#ifndef TB_BUILDER_H
#define TB_BUILDER_H

#include "../types/typeTable.h"
#include "declaration/functionTable.h"
#include "declaration/varTable.h"
#include <memory>
namespace nicole {

class TBBuilder {
private:
  TBBuilder() = delete;

public:
  ~TBBuilder() = default;

  static std::shared_ptr<VariableTable>
  createScope(std::shared_ptr<VariableTable> father = nullptr) {
    return std::make_shared<VariableTable>(father);
  }

  static std::shared_ptr<TypeTable> createTypeTB() {
    return std::make_shared<TypeTable>();
  }

  static std::shared_ptr<FunctionTable> createFunctTB() {
    return std::make_shared<FunctionTable>();
  }
};

} // namespace nicole

#endif