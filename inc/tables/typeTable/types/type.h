#ifndef BASE_TYPE_H
#define BASE_TYPE_H

#include <string>

namespace nicole {

class Type {
public:
  virtual ~Type() = default;

  [[nodiscard]] virtual std::string toString() const noexcept = 0;
};

}

#endif