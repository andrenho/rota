#ifndef ROTA_ROTAVM_HH
#define ROTA_ROTAVM_HH

#include <cstdint>
#include <cstddef>

#include <array>
#include <string>
#include <variant>
#include <vector>

#include "value.hh"
#include "vm/compiler/compiler.hh"
#include "optable.hh"

#define STACK_SZ 512

namespace rotavm {

class RotaVM {
public:
    RotaVM();

    // execution
    void set_executable(Executable const& exec);
    void run_until_halt();

    // debug
    [[nodiscard]] Value const& last_value() const { return last_value_; }
    [[nodiscard]] size_t       stack_sz() const { return stack_.size(); }
    [[nodiscard]] std::string  debug_stack() const;

private:
    struct Address {
        FunctionId f_id;
        size_t     addr;
    };

    Executable           exec_;              // executable code
    std::vector<Value>   stack_ {};          // operational stack
    std::stack<Address>  call_stack_;        // stack of calls

    std::vector<Value>   locals_vars_;       // stack of variables
    std::stack<size_t>   local_vars_fp_;     //   - frame pointer
    std::vector<Value>   global_vars_;       // list of globals

    Value                last_value_;        // last pushed value (used only for debugging)
    const OpTable        op_table;           // table of operations (ALU)

    bool step();

    void function_call(size_t param_count);
    void return_from_function();

    // stack manipulation
    void         push(Value const& value);
    void         push(bool v) { push(Value(v ? -1 : 0)); }
    Value        pop();
    Value        peek();

    [[nodiscard]] FunctionId current_function() const { return call_stack_.top().f_id; }
    [[nodiscard]] size_t& addr() { return call_stack_.top().addr; }
};

}

#endif //ROTA_ROTAVM_HH
