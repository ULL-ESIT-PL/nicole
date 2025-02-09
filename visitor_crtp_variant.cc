#include <iostream>
#include <memory>
#include <string>
#include <variant>

// Forward declarations para la recursividad.
struct AST_BINARY;
struct AST_IF;

// CRTP base para los nodos AST.
template <typename Derived>
class AST_Crtp {
public:
  // Operador de conversión genérico: invoca toValue() del derivado.
  // Por defecto, se espera que T sea double.
  template <typename T = double>
  operator T() const {
    return static_cast<const Derived*>(this)->toValue();
  }
};

// Nodo hoja: almacena un valor double.
struct AST_DOUBLE : public AST_Crtp<AST_DOUBLE> {
  double value;
  AST_DOUBLE(double value) : value(value) {}
  template <typename T = double>
  T toValue() const {
    return static_cast<T>(value);
  }
};

// Nodo hoja: almacena un valor int.
struct AST_INT : public AST_Crtp<AST_INT> {
  int value;
  AST_INT(int value) : value(value) {}
  template <typename T = int>
  T toValue() const {
    return static_cast<T>(value);
  }
};

// Nodo hoja: almacena un valor bool.
struct AST_BOOL : public AST_Crtp<AST_BOOL> {
  bool value;
  AST_BOOL(bool value) : value(value) {}
  template <typename T = bool>
  T toValue() const {
    return static_cast<T>(value);
  }
};

// Nodo hoja: almacena una cadena de texto.
struct AST_STRING : public AST_Crtp<AST_STRING> {
  std::string value;
  AST_STRING(const std::string &value) : value(value) {}
  // Si se solicita como std::string se retorna la cadena;
  // si se solicita como double se intenta convertirla (o retorna 0 en caso de error).
  template <typename T = std::string>
  T toValue() const {
    if constexpr (std::is_same_v<T, std::string>)
      return value;
    else {
      try {
        return static_cast<T>(std::stod(value));
      } catch (...) {
        return static_cast<T>(0);
      }
    }
  }
};

// Se define el variant que contendrá un AST_DOUBLE, AST_INT, AST_BOOL,
// AST_STRING o un puntero a AST_BINARY o AST_IF.
using ASTNode = std::variant<AST_DOUBLE, AST_INT, AST_BOOL, AST_STRING,
                             std::unique_ptr<AST_BINARY>,
                             std::unique_ptr<AST_IF>>;

// Nodo binario: representa la suma de sus hijos.
struct AST_BINARY : public AST_Crtp<AST_BINARY> {
  ASTNode left;
  ASTNode right;
  // Se reciben los ASTNode por valor y se mueven para evitar copiar el unique_ptr.
  AST_BINARY(ASTNode left, ASTNode right)
      : left(std::move(left)), right(std::move(right)) {}
  template <typename T = double>
  T toValue() const {
    auto visitor = [](const auto &node) -> T {
      return static_cast<T>(node);
    };
    T l = std::visit(visitor, left);
    T r = std::visit(visitor, right);
    return l + r;
  }
};

// Nodo if: representa un condicional if-then.
// Evalúa la condición y, si es verdadera (distinta de 0), retorna la evaluación de la rama verdadera;
// en caso contrario, retorna 0.
struct AST_IF : public AST_Crtp<AST_IF> {
  ASTNode condition;
  ASTNode trueBranch;
  AST_IF(ASTNode condition, ASTNode trueBranch)
      : condition(std::move(condition)), trueBranch(std::move(trueBranch)) {}
  template <typename T = double>
  T toValue() const {
    double condVal = std::visit([](const auto &node) -> double {
      return static_cast<double>(node);
    }, condition);
    if (condVal != 0)
      return static_cast<T>(std::visit([](const auto &node) -> double {
        return static_cast<double>(node);
      }, trueBranch));
    else
      return 0;
  }
};

// Helper para combinar lambdas.
template <typename... Base>
struct Visitor : Base... {
  using Base::operator()...;
};
template <typename... T>
Visitor(T...) -> Visitor<T...>;

// Función para imprimir el árbol usando lambdas separadas para cada tipo.
std::string printTree(const ASTNode &node) {
  auto visitor = Visitor{
    // Para AST_DOUBLE.
    [](const AST_DOUBLE &n) -> std::string {
      return std::to_string(static_cast<double>(n));
    },
    // Para AST_INT.
    [](const AST_INT &n) -> std::string {
      return std::to_string(static_cast<int>(n));
    },
    // Para AST_BOOL.
    [](const AST_BOOL &n) -> std::string {
      return static_cast<bool>(n) ? "true" : "false";
    },
    // Para AST_STRING: se muestra entre comillas.
    [](const AST_STRING &n) -> std::string {
      return "\"" + n.toValue<std::string>() + "\"";
    },
    // Para AST_BINARY.
    [](const std::unique_ptr<AST_BINARY> &n) -> std::string {
      return "(" + printTree(n->left) + " + " + printTree(n->right) + ")";
    },
    // Para AST_IF.
    [](const std::unique_ptr<AST_IF> &n) -> std::string {
      return "if(" + printTree(n->condition) + ") then " + printTree(n->trueBranch);
    }
  };
  return std::visit(visitor, node);
}

// Función para evaluar el árbol usando lambdas separadas para cada tipo.
double evaluateTree(const ASTNode &node) {
  auto visitor = Visitor{
    [](const AST_DOUBLE &n) -> double {
      return static_cast<double>(n);
    },
    [](const AST_INT &n) -> double {
      return static_cast<double>(n);
    },
    [](const AST_BOOL &n) -> double {
      return static_cast<double>(n);
    },
    // Para AST_STRING se intenta convertir la cadena a número; si falla, retorna 0.
    [](const AST_STRING &n) -> double {
      try {
        return std::stod(n.toValue<std::string>());
      } catch (...) {
        return 0;
      }
    },
    [](const std::unique_ptr<AST_BINARY> &n) -> double {
      return evaluateTree(n->left) + evaluateTree(n->right);
    },
    [](const std::unique_ptr<AST_IF> &n) -> double {
      if (evaluateTree(n->condition) != 0)
        return evaluateTree(n->trueBranch);
      return 0;
    }
  };
  return std::visit(visitor, node);
}

int main() {
  // Se crean nodos hoja.
  AST_DOUBLE leaf1{25.99};
  AST_INT leaf2{10};
  AST_BOOL leaf3{false};
  AST_STRING leafString{"3.14"};
  AST_DOUBLE leaf4{5.0};
  AST_INT leaf5{2};
  AST_INT leaf6{100};

  // Convertir hojas a ASTNode.
  ASTNode node_leaf1 = leaf1;
  ASTNode node_leaf2 = leaf2;
  ASTNode node_leaf3 = leaf3;
  ASTNode node_leafStr = leafString;
  ASTNode node_leaf4 = leaf4;
  ASTNode node_leaf5 = leaf5;
  ASTNode node_leaf6 = leaf6;

  // Se crea un nodo interno: (25.99 + 10)
  auto inner1 = std::make_unique<AST_BINARY>(std::move(node_leaf1),
                                             std::move(node_leaf2));
  ASTNode node_inner1 = std::move(inner1);

  // Se crea otro nodo: ((25.99 + 10) + false)
  auto inner2 = std::make_unique<AST_BINARY>(std::move(node_inner1),
                                             std::move(node_leaf3));
  ASTNode node_inner2 = std::move(inner2);

  // Se suma la cadena: (((25.99 + 10) + false) + "3.14")
  auto inner3 = std::make_unique<AST_BINARY>(std::move(node_inner2),
                                             std::move(node_leafStr));
  ASTNode node_inner3 = std::move(inner3);

  // Rama verdadera: (5.0 + 2)
  auto trueBranch = std::make_unique<AST_BINARY>(std::move(node_leaf4),
                                                 std::move(node_leaf5));
  ASTNode node_trueBranch = std::move(trueBranch);

  // Se crea el nodo if: if(((25.99 + 10) + false + "3.14")) then (5.0 + 2)
  auto ifNode = std::make_unique<AST_IF>(std::move(node_inner3),
                                         std::move(node_trueBranch));
  ASTNode node_if = std::move(ifNode);

  std::cout << "Print Tree: " << printTree(node_if) << "\n";
  std::cout << "Evaluate Tree: " << evaluateTree(node_if) << "\n";

  return 0;
}
