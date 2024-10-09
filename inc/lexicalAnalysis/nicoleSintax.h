#ifndef NICOLE_SINTAX_H
#define NICOLE_SINTAX_H

#include "sintax.h"

namespace nicole {

// The sintax used for the Nicole language
class NicoleSintax final : public Sintax {
 public:
  NicoleSintax() = default;

  Lexer createLexer() const override;
};

}  // namespace nicole

#endif