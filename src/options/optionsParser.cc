#include "../../inc/options/optionsParser.h"
#include <cstddef>
#include <string_view>

namespace nicole {

bool OptionParser::isOption(const std::string_view argument) {
  if (argument == "-o" or argument == "--optimize" or argument == "-n" or
      argument == "--name" or argument == "-h" or argument == "--help") {
    return true;
  }
  return false;
}

std::expected<Options, Error>
OptionParser::parse(const std::vector<std::string_view> &arguments) {
  bool help{false};
  bool optimize{false};
  std::string binaryName{"a.out"};
  std::filesystem::path entryFilePath{""};

  bool entryFileParsed{false};
  const std::size_t size{arguments.size()};

  if (!size) {
    return std::unexpected{
        Error{ERROR_TYPE::MISSING_ARGUMENTS,
              "at least must give the name of the entry file"}};
  }

  for (std::size_t i{0}; i < size; ++i) {
    const std::string_view argument{arguments[i]};
    if (argument == "-h" or argument == "--help") {
      help = true;
    } else if (argument == "-o" or argument == "--optimize") {
      optimize = true;
    } else if (argument == "-n" or argument == "--name") {
      if (++i == size or isOption(arguments[i])) {
        return std::unexpected{
            Error{ERROR_TYPE::MISSING_BINARY_NAME, "missing binary name"}};
      }
      binaryName = arguments[i];
    } else {
      if (entryFileParsed) {
        return std::unexpected{
            Error{ERROR_TYPE::UNKOWN_ARGUMENT,
                  "argument: " + std::string{argument} + " not supported"}};
      }
      entryFilePath = argument;
      entryFileParsed = true;
    }
  }

  if (entryFilePath.empty()) {
    return std::unexpected{
        Error{ERROR_TYPE::MISSING_ENTRY_FILE, "must specify the entry file"}};
  }

  return Options{help, optimize, binaryName, entryFilePath};
}

} // namespace nicole