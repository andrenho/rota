#ifndef ROTA_VALUE_HH
#define ROTA_VALUE_HH

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "type.hh"

namespace rotavm {

using FunctionId = size_t;

struct Function {
    explicit Function(FunctionId id) : id(id) {}

    FunctionId id;

    bool operator==(Function const& rhs) const {
        return id == rhs.id;
    }
};

class Value {
public:
    Value() : type_(T_NIL), i_(0) {}
    explicit Value(bool b) : type_(T_INT), i_(b ? -1 : 0) {}
    explicit Value(int32_t i) : type_(T_INT), i_(i) {}
    explicit Value(float f) : type_(T_FLOAT), f_(f) {}
    explicit Value(Function const& fn) : type_(T_FUNCTION), fn_(fn) {}

    [[nodiscard]] Type       type() const { return type_; }
    [[nodiscard]] int        i() const { return i_; }
    [[nodiscard]] float      f() const { return f_; }
    [[nodiscard]] FunctionId functionId() const { return fn_.id; }

    [[nodiscard]] std::string debug() const
    {
        switch (type_) {
            case T_NIL: return "nil";
            case T_INT: return std::to_string(i_);
            case T_FLOAT: return std::to_string(f_);
            case T_FUNCTION: return "fn(" + std::to_string(fn_.id) + ")";
            default: throw std::runtime_error("not implemented");
        }
    }

private:
    Type type_;
    union {
        int32_t  i_;
        float    f_;
        Function fn_;
    };
};

}

#endif //ROTA_VALUE_HH
