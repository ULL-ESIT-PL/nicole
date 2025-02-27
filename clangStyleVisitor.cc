#include <cstdlib>
#include <iostream>
#include <memory>

namespace clangStyle {

// --- Definición del AST ---

class ASTNode {
public:
  virtual ~ASTNode() = default;
};

class NumNode : public ASTNode {
public:
  int value;
  explicit NumNode(int v) : value(v) {}
};

class AddNode : public ASTNode {
public:
  std::unique_ptr<ASTNode> left;
  std::unique_ptr<ASTNode> right;
  AddNode(std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
      : left(std::move(l)), right(std::move(r)) {}
};

// --- Visitor basado en CRTP ---

template <typename Derived> class RecursiveASTVisitor {
public:
  bool TraverseAST(ASTNode *node) {
    if (!node)
      return true;
    // Se despacha según el tipo dinámico del nodo.
    if (auto *num = dynamic_cast<NumNode *>(node))
      return static_cast<Derived *>(this)->VisitNum(num);
    if (auto *add = dynamic_cast<AddNode *>(node))
      return static_cast<Derived *>(this)->VisitAdd(add);
    return true;
  }
};

class EvalVisitor : public RecursiveASTVisitor<EvalVisitor> {
public:
  int result = 0;

  bool VisitNum(NumNode *node) {
    result = node->value;
    return true;
  }

  bool VisitAdd(AddNode *node) {
    int leftVal = 0, rightVal = 0;
    {
      EvalVisitor lv;
      lv.TraverseAST(node->left.get());
      leftVal = lv.result;
    }
    {
      EvalVisitor rv;
      rv.TraverseAST(node->right.get());
      rightVal = rv.result;
    }
    result = leftVal + rightVal;
    return true;
  }
};

} // namespace clangStyle

// --- Función main para probar el AST y el visitor ---

int main() {
  using namespace clangStyle;
  // Se construye el AST para la expresión: (3 + 4)
  std::unique_ptr<ASTNode> expr = std::make_unique<AddNode>(
      std::make_unique<NumNode>(3), std::make_unique<NumNode>(4));

  EvalVisitor evaluator;
  evaluator.TraverseAST(expr.get());
  std::cout << "Resultado: " << evaluator.result
            << std::endl; // Imprime: Resultado: 7

  return EXIT_SUCCESS;
}
