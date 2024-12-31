#ifndef AST_IMPORT_H
#define AST_IMPORT_H

#include "../ast.h"
#include <filesystem>
#include <memory>

namespace nicole {

class AST_IMPORT : public AST {
private:
  std::filesystem::path path_;

public:
  explicit AST_IMPORT(const std::filesystem::path &path,
                      const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::IMPORT, sourceLocation), path_{path} {}

  [[nodiscard]] const std::filesystem::path &path() const noexcept {
    return path_;
  }
};

} // namespace nicole

#endif