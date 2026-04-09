#ifndef ROTA_ROTAVM_HH
#define ROTA_ROTAVM_HH

#include <cstdint>
#include <cstddef>

#include <array>
#include <string>
#include <variant>

#include "value.hh"
#include "vm/compiler/compiler.hh"
#include "optable.hh"

#define STACK_SZ 512

namespace rotavm {

class RotaVM {
public:
    // execution
    void set_executable(Executable const& exec) { exec_ = exec; }
    void run_until_halt();

    // debug
    [[nodiscard]] Value const& last_value() const { return last_value_; }
    [[nodiscard]] size_t       stack_sz() const { return stack_idx_; }
    [[nodiscard]] std::string  debug_stack() const;

private:
    std::array<Value, STACK_SZ> stack_ {};
    size_t                      stack_idx_ = 0;
    Executable                  exec_;
    FunctionId                  current_function_ = 0;
    Value                       last_value_;
    uint32_t                    PC_ = 0;

    struct CallStackElement {
        FunctionId f_id;
        uint32_t   PC;
    };
    std::stack<CallStackElement> call_stack_;

    const OpTable op_table;

    bool step();

    // stack manipulation
    void         push(Value const& value);
    void         push(bool v) { push(Value(v ? -1 : 0)); }
    Value        pop();
    Value        peek();
};

}

#endif //ROTA_ROTAVM_HH
