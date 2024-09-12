#ifndef SINTAX_H
#define SINTAX_H

#include "lexer.h"

namespace nicole {

class Sintax {
 public:
   virtual ~Sintax() = default;

   virtual Lexer createLexer() const = 0;
};

}  // namespace nicole

#endif