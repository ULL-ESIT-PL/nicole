#ifndef GENERIC_TYPE_H
#define GENERIC_TYPE_H

#include <memory>
#include <string>

namespace nicole {

class GenericType {
 private:
  /* data */
  std::string name_{""};
  std::unique_ptr<GenericType> father_;

 public:
  GenericType(const std::string& name, std::unique_ptr<GenericType> father = nullptr)
      : name_{name}, father_{std::move(father)} {};

  virtual ~GenericType() = default;

  std::string name() const { return name_; }

  GenericType* father() const { return father_.get(); }
};

}  // namespace nicole

#endif