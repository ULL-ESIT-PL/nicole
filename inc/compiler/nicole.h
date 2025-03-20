#ifndef NICOLE_H
#define NICOLE_H

#include "compiler.h"

namespace nicole {

class Nicole final : public Compiler {
public:
    explicit Nicole(const std::shared_ptr<Sintax> &sintax) noexcept : Compiler(sintax) {}

  [[nodiscard]] std::expected<std::monostate, Error>
  compile(const Options &options) const noexcept override;
};

} // namespace nicole

#endif