#include "../../inc/compiler/nicole.h"
#include <memory>

namespace nicole {

std::expected<std::monostate, Error>
Nicole::compile(const Options &options) const noexcept {
  const nicole::TopDown topDown{sintax_};

  const std::expected<std::shared_ptr<nicole::Tree>, nicole::Error> tree{
      topDown.parse(options.entryFilePath())};

  if (!tree) {
    return createError(tree.error());
  }

  std::cout << "Finished parsing\n";

  if (options.validateTree()) {
    const nicole::ValidateTree validateTree{};
    const std::expected<bool, nicole::Error> validated{
        validateTree.validate((*tree).get())};

    if (!validated) {
      return createError(validated.error());
    }
    std::cout << "Finished validate\n";
  }

  if (options.printTree()) {
    const nicole::PrintTree printTree{};
    const std::expected<std::string, nicole::Error> toStr{
        printTree.print((*tree).get())};
    if (!toStr) {
      return createError(toStr.error());
    }
    std::cout << *toStr << "\n";
    std::cout << "Finished print tree\n";
  }

  std::shared_ptr<nicole::TypeTable> typeTable{
      std::make_shared<nicole::TypeTable>()};
  std::shared_ptr<nicole::FunctionTable> functionTable{
      std::make_shared<nicole::FunctionTable>()};
  const nicole::FillSemanticInfo semanticFiller{functionTable, typeTable,
                                                options};
  const auto isTablesFilled{semanticFiller.fill((*tree).get())};
  if (!isTablesFilled) {
    return createError(isTablesFilled.error());
  }

  std::cout << "Finished semantic analysis\n";

  const nicole::TypeAnalysis typeAnalysis{functionTable, typeTable};
  const auto analyzed{typeAnalysis.analyze((*tree).get())};
  if (!analyzed) {
    return createError(analyzed.error());
  }

  std::cout << "Finished type analysis\n";

  return {};
}

} // namespace nicole