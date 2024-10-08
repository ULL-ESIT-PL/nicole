#include "inc/parsingAnalysis/ast/declaration/varTable.h"
#include <memory>

int main() {
    auto father{std::make_shared<nicole::VariableTable>(nullptr)};
    father->addVariable("x", nullptr, nullptr, nullptr);
    auto child{std::make_shared<nicole::VariableTable>(father)};
    std::cout << child->hasVariable("x");
}