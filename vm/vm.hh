#ifndef ROTA_VM_HH
#define ROTA_VM_HH

#include <functional>
#include "assembly/bytecode.hh"

class VM {
public:
    void set_bytecode(Bytecode&& bytecode) { bytecode_ = std::move(bytecode); }

    void run(std::function<void()> before_each_instruction);
    void run_debug_console();

private:
    Bytecode bytecode_;
};

#endif //ROTA_VM_HH
