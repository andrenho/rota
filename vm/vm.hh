#ifndef ROTA_VM_HH
#define ROTA_VM_HH

#include <functional>
#include <memory>

#include "bytecode/bytecode.hh"

class VM {
public:
    using Value = std::variant<int32_t>;

    void set_bytecode(std::unique_ptr<Bytecode> bytecode) { bytecode_ = std::move(bytecode); }

    void run(std::function<void(size_t)> const& after_each_instruction=[](size_t){});
    void run_debug_console();

    [[nodiscard]] Value last_value_popped() const { return last_value_popped_; }
    [[nodiscard]] size_t stack_sz() const { return stack_.size(); }
    [[nodiscard]] std::string debug_stack() const;

private:
    std::unique_ptr<Bytecode> bytecode_;
    size_t                    pc_ = 0;
    std::vector<Value>        stack_;
    Value                     last_value_popped_;

    void step();
};

#endif //ROTA_VM_HH
