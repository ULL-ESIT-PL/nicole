#ifndef GENERIC_TYPE_H
#define GENERIC_TYPE_H

#include <memory>
#include <string>

namespace nicole {

class GenericType {
private:
  /* data */
  std::string name_{""};
  std::weak_ptr<GenericType> father_;

public:
  GenericType(const std::string &name,
              std::shared_ptr<GenericType> father = nullptr)
      : name_{name}, father_{father} {};

  virtual ~GenericType() = default;

  std::string name() const { return name_; }

  std::shared_ptr<GenericType> father() const { return father_.lock(); }
};

} // namespace nicole

#endif