#ifndef NICOLE_SINTAX_H
#define NICOLE_SINTAX_H

#include "lexer.h"

namespace nicole {

class NicoleSintax final {
 private:
    NicoleSintax() = delete;
 public:
  static Lexer createLexer();
};

}  // namespace nicole

#endif