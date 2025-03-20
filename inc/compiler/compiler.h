#ifndef COMPILER_H
#define COMPILER_H

#include "../lexicalAnalysis/nicoleSintax.h"
#include "../options/optionsParser.h"
#include "../parsingAnalysis/algorithm/topDown.h"
#include "../visitors/codeGeneration.h"
#include "../visitors/fillSemanticInfo.h"
#include "../visitors/monomorphize.h"
#include "../visitors/printTree.h"
#include "../visitors/typeAnalysis.h"
#include "../visitors/validateTree.h"
#include <expected>
#include <iostream>
#include <memory>
#include <variant>

namespace nicole {

class Compiler final {
private:
  std::shared_ptr<Sintax> sintax_;

public:
  explicit Compiler(const std::shared_ptr<Sintax> &sintax) noexcept
      : sintax_{sintax} {}

  [[nodiscard]] const std::shared_ptr<Sintax> &sintax() const noexcept {
    return sintax_;
  }

  [[nodiscard]] std::expected<std::monostate, Error>
  compile(const Options &options) const noexcept;
};

} // namespace nicole

#endif