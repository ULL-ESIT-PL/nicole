#ifndef TYPED_EXPRESSION_H
#define TYPED_EXPRESSION_H

#include "../../types/genericType.h"

namespace nicole {

class TypedExpression {
  public:
    virtual std::shared_ptr<GenericType> type() const = 0;   
};

}

#endif