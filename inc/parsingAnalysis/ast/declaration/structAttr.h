#ifndef PARAMS_DECLARATION_H
#define PARAMS_DECLARATION_H

#include "../declaration/varTable.h"
#include "../node.h"
#include <memory>
#include <utility>
#include <vector>

namespace nicole {

class ParamsDeclaration {
private:
  /* data */
  std::vector<std::pair<std::string, std::string>> params_;
  std::shared_ptr<VariableTable> currentScope_;

public:
  ParamsDeclaration(
      std::vector<std::pair<std::string, std::string>> params,
      std::shared_ptr<VariableTable> currentScope)
      : params_{params}, currentScope_{currentScope} {};

  std::vector<std::pair<std::string, std::string>>
  paramters() const {
    return params_;
  }

  auto begin() const { return params_.begin(); }

  auto end() const { return params_.end(); }

  std::shared_ptr<VariableTable> table() const { return currentScope_; }
};

} // namespace nicole

#endif