#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>

namespace nicole {

class VariableTable final {
 private:
  std::unique_ptr<VariableTable> father_;
  std::unordered_map<std::string, std::tuple<>> table_{};

 public:
  VariableTable(std::unique_ptr<VariableTable> father = nullptr)
      : father_{std::move(father)} {};
};

}  // namespace nicole

#endif