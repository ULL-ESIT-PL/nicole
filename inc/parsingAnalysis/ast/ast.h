#ifndef AST_H
#define AST_H

#include <memory>

#include "astType.h"

namespace nicole {

class AST {
protected:
    AST_TYPE type_;
    std::weak_ptr<AST> father_;

public:
    explicit AST(const AST_TYPE type, const std::shared_ptr<AST>& father) noexcept : type_{type}, father_{father} {}

    AST_TYPE type() const { return type_; }

    // const std::shared_ptr<AST> father() const { return father_; }
};

}

#endif