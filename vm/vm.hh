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

    [[nodiscard]] size_t stack_sz() const { return stack_.size(); }
    [[nodiscard]] std::string debug_stack() const;
    [[nodiscard]] Value const& debug_saved() const { return debug_saved_; }

private:
    std::unique_ptr<Bytecode> bytecode_;
    size_t                    pc_ = 0;
    std::vector<Value>        stack_;
    Value                     debug_saved_;

    void step();
};

namespace std {
std::string to_string(VM::Value const& value);
};

#endif //ROTA_VM_HH
