#ifndef SINTAX_H
#define SINTAX_H

#include "lexer.h"

namespace nicole {

// Abstract class that represents the sintax of a language
class Sintax {
 public:
  virtual ~Sintax() = default;

  virtual Lexer createLexer() const = 0;
};

}  // namespace nicole

#endif