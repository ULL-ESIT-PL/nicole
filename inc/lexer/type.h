#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include <string>

namespace nicole {
class Type final {
 private:
  std::string type_{""};

 public:
  Type(const std::string& type) : type_{type} {};
  std::string name() const { return type_; }
};

}  // namespace nicole

#endif