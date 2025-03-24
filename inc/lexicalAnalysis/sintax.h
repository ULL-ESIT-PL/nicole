#ifndef SINTAX_H
#define SINTAX_H

#include "lexer.h"
#include <string>

namespace nicole {

// Abstract class that represents the sintax of a language
class Sintax {
private:
  std::string extension_;

public:
  explicit Sintax(const std::string &extension) noexcept
      : extension_{extension} {}

  virtual ~Sintax() = default;

  [[nodiscard]] const std::string &extension() const noexcept {
    return extension_;
  }

  [[nodiscard]] virtual const Lexer createLexer() const noexcept = 0;
};

} // namespace nicole

#endif