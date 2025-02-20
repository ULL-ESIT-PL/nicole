#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// Enumeración para tipos básicos
enum class BasicKind { Bool, Int, Float, Double, Char, Str };

// Clase base abstracta para todos los tipos
class Type {
public:
  virtual ~Type() = default;
  virtual std::string toString() const = 0;
};

// Tipo null (similar a std::nullptr_t)
class NullType : public Type {
public:
  std::string toString() const override { return "nullptr"; }
};

// Tipo void
class VoidType : public Type {
public:
  std::string toString() const override { return "void"; }
};

// Tipo básico
class BasicType : public Type {
  BasicKind kind;

public:
  explicit BasicType(BasicKind k) : kind(k) {}
  std::string toString() const override {
    switch (kind) {
    case BasicKind::Int:
      return "int";
    case BasicKind::Float:
      return "float";
    case BasicKind::Char:
      return "char";
    case BasicKind::Bool:
      return "bool";
    }
    return "";
  }
};

// Tipo puntero
class PointerType : public Type {
  std::unique_ptr<Type> baseType;

public:
  explicit PointerType(std::unique_ptr<Type> base)
      : baseType(std::move(base)) {}
  std::string toString() const override { return baseType->toString() + "*"; }
};

// Tipo vector
class VectorType : public Type {
  std::unique_ptr<Type> elementType;

public:
  explicit VectorType(std::unique_ptr<Type> elem)
      : elementType(std::move(elem)) {}
  std::string toString() const override {
    return "vector<" + elementType->toString() + ">";
  }
};

// Tipo constante
class ConstType : public Type {
  std::unique_ptr<Type> baseType;

public:
  explicit ConstType(std::unique_ptr<Type> base) : baseType(std::move(base)) {}
  std::string toString() const override {
    return "const " + baseType->toString();
  }
};

// Representación de un parámetro genérico
class GenericParameter {
  std::string name;

public:
  explicit GenericParameter(std::string n) : name(std::move(n)) {}
  std::string toString() const { return name; }
};

// Tipo de usuario con soporte de herencia y parámetros genéricos
class UserType : public Type {
  std::string name;
  std::vector<UserType *> baseTypes;
  std::vector<GenericParameter> genericParams;

public:
  explicit UserType(std::string n) : name(std::move(n)) {}

  // Agregar herencia
  void addBase(UserType *base) { baseTypes.push_back(base); }

  // Agregar un parámetro genérico
  void addGenericParam(GenericParameter param) {
    genericParams.push_back(param);
  }

  std::string toString() const override {
    std::ostringstream oss;
    oss << name;
    if (!genericParams.empty()) {
      oss << "<";
      for (size_t i = 0; i < genericParams.size(); ++i) {
        oss << genericParams[i].toString();
        if (i != genericParams.size() - 1)
          oss << ", ";
      }
      oss << ">";
    }
    return oss.str();
  }
};

// Representación de una instanciación de un tipo genérico con argumentos
// concretos
class GenericInstanceType : public Type {
  std::unique_ptr<UserType> genericType;
  std::vector<std::unique_ptr<Type>> typeArgs;

public:
  GenericInstanceType(std::unique_ptr<UserType> genType,
                      std::vector<std::unique_ptr<Type>> args)
      : genericType(std::move(genType)), typeArgs(std::move(args)) {}

  std::string toString() const override {
    std::ostringstream oss;
    oss << genericType->toString() << "(";
    for (size_t i = 0; i < typeArgs.size(); ++i) {
      oss << typeArgs[i]->toString();
      if (i != typeArgs.size() - 1)
        oss << ", ";
    }
    oss << ")";
    return oss.str();
  }
};

int main() {
  // Definir un tipo de usuario genérico: MyClass<T>
  auto userType = std::make_unique<UserType>("MyClass");
  userType->addGenericParam(GenericParameter("T"));
  userType->addGenericParam(GenericParameter("Q"));
  std::cout << userType->toString() << std::endl; // Salida: MyClass<T>

  // Instanciación del tipo genérico con un argumento concreto (int)
  std::vector<std::unique_ptr<Type>> args;
  args.push_back(std::make_unique<BasicType>(BasicKind::Int));
  args.push_back(std::make_unique<BasicType>(BasicKind::Bool));
  GenericInstanceType instance(std::move(userType), std::move(args));
  std::cout << instance.toString() << std::endl; // Salida: MyClass<T>(int)

  return 0;
}
