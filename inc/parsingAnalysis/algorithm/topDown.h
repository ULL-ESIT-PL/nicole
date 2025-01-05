#ifndef TOP_DOWN_H
#define TOP_DOWN_H

#include "parser.h"

namespace nicole {

class TopDown final : public Parser {
public:
  explicit TopDown(const std::shared_ptr<Sintax> &sintax) noexcept
      : Parser{sintax} {}

  [[nodiscard]] const std::expected<std::shared_ptr<Tree>, Error>
  parse(const std::filesystem::path &entryFile) const noexcept override;
};

} // namespace nicole

#endif