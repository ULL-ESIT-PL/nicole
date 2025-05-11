#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include "../../errors.h"
#include "../../lexicalAnalysis/type.h"
#include "types/basicTypes/basicTypes.h"
#include "types/specialTypes/breakType.h"
#include "types/specialTypes/constType.h"
#include "types/specialTypes/noPropagateType.h"
#include "types/specialTypes/nullType.h"
#include "types/specialTypes/ptrType.h"
#include "types/specialTypes/vectorType.h"
#include "types/specialTypes/voidType.h"
#include "types/type.h"
#include "types/userTypes/enumType.h"
#include "types/userTypes/genericInstanceType.h"
#include "types/userTypes/genericParameter.h"
#include "types/userTypes/placeHolder.h"
#include "types/userTypes/userType.h"
#include <expected>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace nicole {

class TypeTable {
private:
  std::unordered_map<std::string, std::shared_ptr<Type>> table_{
      {"bool", std::make_shared<BasicType>(BasicKind::Bool)},
      {"int", std::make_shared<BasicType>(BasicKind::Int)},
      {"float", std::make_shared<BasicType>(BasicKind::Float)},
      {"double", std::make_shared<BasicType>(BasicKind::Double)},
      {"char", std::make_shared<BasicType>(BasicKind::Char)},
      {"str", std::make_shared<BasicType>(BasicKind::Str)},
      {"void", std::make_shared<VoidType>()},
  };

  std::shared_ptr<NullType> null_{std::make_shared<NullType>()};

  std::shared_ptr<NoPropagateType> noPropagate_{
      std::make_shared<NoPropagateType>()};

  std::shared_ptr<BreakType> breakType_{std::make_shared<BreakType>()};

  bool canAssignImpl(const std::shared_ptr<Type> &target,
                     const std::shared_ptr<Type> &source,
                     bool pointerContext) const noexcept;

public:
  [[nodiscard]] const std::shared_ptr<NullType> &null() const noexcept {
    return null_;
  }

  [[nodiscard]] const std::shared_ptr<Type> &boolType() const noexcept {
    return *getType("bool");
  }

  [[nodiscard]] const std::shared_ptr<Type> &intType() const noexcept {
    return *getType("int");
  }

  [[nodiscard]] const std::shared_ptr<Type> &floatType() const noexcept {
    return *getType("float");
  }

  [[nodiscard]] const std::shared_ptr<Type> &doubleType() const noexcept {
    return *getType("double");
  }

  [[nodiscard]] const std::shared_ptr<Type> &charType() const noexcept {
    return *getType("char");
  }

  [[nodiscard]] const std::shared_ptr<Type> &strType() const noexcept {
    return *getType("str");
  }

  [[nodiscard]] const std::shared_ptr<Type> &voidType() const noexcept {
    return *getType("void");
  }

  [[nodiscard]] const std::shared_ptr<NoPropagateType> &
  noPropagateType() const noexcept {
    return noPropagate_;
  }

  [[nodiscard]] const std::shared_ptr<BreakType> &breakType() const noexcept {
    return breakType_;
  }

  [[nodiscard]] bool has(const std::string &id) const noexcept;

  [[nodiscard]] const std::expected<std::shared_ptr<Type>, Error>
  getType(const std::string &id) const noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  insert(const std::shared_ptr<Type> &type) noexcept;

  [[nodiscard]] bool
  isPossibleType(const std::shared_ptr<Type> &type) const noexcept;

  [[nodiscard]] bool
  isGenericType(const std::shared_ptr<Type> &type,
                const std::vector<GenericParameter> &generics) const noexcept;

  [[nodiscard]] std::expected<std::shared_ptr<Type>, Error>
  isCompundEnumType(const std::shared_ptr<Type> &type) const noexcept;

  [[nodiscard]] std::expected<std::shared_ptr<Type>, Error>
  isCompundGenericType(
      const std::shared_ptr<Type> &type,
      const std::vector<GenericParameter> &list) const noexcept;

  [[nodiscard]] bool
  isCompundPlaceHolder(const std::shared_ptr<Type> &type) const noexcept;

  [[nodiscard]] bool
  areSameType(const std::shared_ptr<Type> &type1,
              const std::shared_ptr<Type> &type2) const noexcept;

  [[nodiscard]] bool
  canAssign(const std::shared_ptr<Type> &target,
            const std::shared_ptr<Type> &source) const noexcept;

  [[nodiscard]] bool
  haveCommonAncestor(const std::shared_ptr<Type> &type1,
                     const std::shared_ptr<Type> &type2) const noexcept;

  [[nodiscard]] std::expected<std::shared_ptr<Type>, Error>
  applyUnaryOperator(const std::shared_ptr<Type> &operand,
                     const TokenType op) const noexcept;

  [[nodiscard]] std::expected<std::shared_ptr<Type>, Error>
  applyBinaryOperator(const std::shared_ptr<Type> &left,
                      const std::shared_ptr<Type> &right,
                      TokenType op) const noexcept;

  [[nodiscard]] std::expected<std::shared_ptr<Type>, Error>
  applyGenericReplacements(
      const std::shared_ptr<Type> &type,
      const std::vector<GenericParameter> &genericParams,
      const std::vector<std::shared_ptr<Type>> &replacements) const noexcept;
};

} // namespace nicole

#endif