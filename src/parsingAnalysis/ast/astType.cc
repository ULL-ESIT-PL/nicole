#include "../../../inc/parsingAnalysis/ast/astType.h"

namespace nicole {

std::string astTypeToStr(const AST_TYPE type) {
  switch (type) {
  case AST_TYPE::INT:
    return "INT";
  }
}

} // namespace nicole