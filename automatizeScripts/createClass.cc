#include <algorithm>  // for using transform
#include <cctype>     // for using toupper
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void createHeaderFile(const std::string& classPath = "",
                      const std::string& className = "") {
  const filesystem::path headerPath{"../inc/" + classPath + "/" + className +
                                    ".h"};
  // Ensure that the directory exists, if not create it
  filesystem::create_directories(headerPath.parent_path());

  if (filesystem::exists(headerPath)) {
    throw std::runtime_error("Unable to create the file, already exists");
  }

  std::string UpperClassName{className};
  transform(UpperClassName.begin(), UpperClassName.end(),
            UpperClassName.begin(), ::toupper);

  std::ofstream file(headerPath);
  file << "#ifndef " + (UpperClassName) + "_H" + "\n";
  file << "#define " + UpperClassName + "_H" + "\n\n";
  file << "namespace nicole {\n";
  file << "class " + className + " {\n";
  file << " private:\n\n";
  file << " public:\n";
  file << "  " + className + "() = default;\n";
  file << "  ~" + className + "() = default;\n";
  file << "};\n}\n\n";
  file << "#endif";
  file << " \n";
}

void createImplementationFile(const std::string& classPath = "",
                              const std::string& className = "") {
  const filesystem::path srcPath{"../src/" + classPath + "/" + className + ".cc"};
  // Ensure that the directory exists, if not create it
  filesystem::create_directories(srcPath.parent_path());

  if (filesystem::exists(srcPath)) {
    throw std::runtime_error("Unable to create the file, already exists");
  }

  std::ofstream file(srcPath);
  file << "#include \"../inc/" + classPath + "/" + className + ".h\"";
  file << "\n\nnamespace nicole {\n\n";
  file << "};";
}

void createClassFiles(const std::string& classPath,
                      const std::string& className) {
  createHeaderFile(classPath, className);
  createImplementationFile(classPath, className);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    throw std::runtime_error(
        "Invalid number of arguments, just need to specify the name of the "
        "class and its path as if it started from inc/src");
  }

  // Separate the path and class name (assuming the input is something like
  // "carpeta/className")
  std::string fullPath{argv[1]};
  size_t lastSlash = fullPath.find_last_of('/');
  std::string classPath = fullPath.substr(0, lastSlash);   // the directory path
  if (lastSlash > fullPath.size()) classPath = "";
  std::string className = fullPath.substr(lastSlash + 1);  // the class name
  if (lastSlash > fullPath.size()) className = fullPath;
  std::cout << lastSlash << classPath << "----" << className;

  if (className.empty()) {
    throw std::runtime_error(
        "Invalid format, please provide in the form of 'directory/className'");
  }

  createClassFiles(classPath, className);

  return 0;
}
