#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include "../errors.h"
#include "options.h"
#include <expected>
#include <string_view>
#include <vector>

namespace nicole {

class OptionsParser final {
private:
  OptionsParser() = delete;

  [[nodiscard]] static bool isOption(const std::string_view argument);

public:
  [[nodiscard]] static std::expected<Options, Error>
  parse(const std::vector<std::string_view> &arguments);
};

} // namespace nicole

#endif