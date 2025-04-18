#ifndef ERROR_TABLE_H
#define ERROR_TABLE_H

#include "../../errors.h"
#include "../../lexicalAnalysis/sourceLocation.h"
#include <cstddef>
#include <expected>
#include <map>

namespace nicole {

class ErrorTable {
private:
  mutable std::map<SourceLocation, Error> errors_;

public:
  [[nodiscard]] std::size_t size() const noexcept { return errors_.size(); }

  void insert(const SourceLocation &srcLoc, const Error &err) const noexcept {
    errors_[srcLoc] = err;
  }

  [[nodiscard]] auto begin() const noexcept { return errors_.begin(); }

  [[nodiscard]] auto end() const noexcept { return errors_.end(); }
};

} // namespace nicole

#endif