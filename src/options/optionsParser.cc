#include "../../inc/options/optionsParser.h"

namespace nicole {

bool OptionsParser::isOption(const std::string_view argument) noexcept {
  if (argument == "-o" or argument == "--optimize" or argument == "-n" or
      argument == "--name" or argument == "-h" or argument == "--help") {
    return true;
  }
  return false;
}

const std::expected<Options, Error>
OptionsParser::parse(const std::vector<std::string_view> &arguments) noexcept {
  const std::size_t size{arguments.size()};

  if (!size) {
    return std::unexpected{
        Error{ERROR_TYPE::MISSING_ARGUMENTS,
              "at least must give the name of the entry file"}};
  }

  bool help{false};
  bool optimize{false};
  std::string binaryName{"a.out"};
  std::filesystem::path entryFilePath{""};

  bool entryFileParsed{false};

  for (std::size_t i{0}; i < size; ++i) {
    const std::string_view argument{arguments[i]};
    if (argument == "-h" or argument == "--help") {
      help = true;
      // break; not using break in case that we let the user use help and
      // execute
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
  // in case that help exists we skip the error
  if (entryFilePath.empty() and !help) {
    return std::unexpected{
        Error{ERROR_TYPE::MISSING_ENTRY_FILE, "must specify the entry file"}};
  }

  return Options{help, optimize, binaryName, entryFilePath};
}

} // namespace nicole