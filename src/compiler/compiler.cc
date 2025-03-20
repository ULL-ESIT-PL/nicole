#include "../../inc/compiler/compiler.h"

namespace nicole {

std::expected<std::monostate, Error>
Compiler::compile(const Options &options) const noexcept {
  const nicole::TopDown topDown{sintax_};

  const std::expected<std::shared_ptr<nicole::Tree>, nicole::Error> tree{
      topDown.parse(options.entryFilePath())};

  if (!tree) {
    return createError(tree.error());
  }

  if (options.validateTree()) {
    const nicole::ValidateTree validateTree{};
    const std::expected<bool, nicole::Error> validated{
        validateTree.validate((*tree).get())};

    if (!validated) {
      return createError(validated.error());
    }
  }

  if (options.printTree()) {
    const nicole::PrintTree printTree{};
    const std::expected<std::string, nicole::Error> toStr{
        printTree.print((*tree).get())};
    if (!toStr) {
      return createError(toStr.error());
    }
    std::cout << *toStr << "\n";
  }

  std::shared_ptr<nicole::FunctionTable> functionTable{
      std::make_shared<nicole::FunctionTable>()};
  std::shared_ptr<nicole::TypeTable> typeTable{
      std::make_shared<nicole::TypeTable>()};
  const nicole::FillSemanticInfo semanticFiller{functionTable, typeTable,
                                                options.validateTree()};
  const auto isTablesFilled{semanticFiller.fill((*tree).get())};
  if (!isTablesFilled) {
    return createError(isTablesFilled.error());
  }
  return {};
}

} // namespace nicole