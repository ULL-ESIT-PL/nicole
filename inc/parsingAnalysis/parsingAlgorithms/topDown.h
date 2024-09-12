#ifndef TOP_DOWN_H
#define TOP_DOWN_H

#include "parser.h"

namespace nicole {
class TopDown final : public Parser {
 private:
  mutable std::unique_ptr<NodeStatement> root_{nullptr};

  std::unique_ptr<Node> parseStart() const;
  std::unique_ptr<Node> parseFactor() const;

 public:
  TopDown(std::unique_ptr<Sintax> sintax): Parser{std::move(sintax)} {}

  ~TopDown() = default;

  std::unique_ptr<Node> parse(const std::filesystem::path& path) const override;
};
}  // namespace nicole

#endif