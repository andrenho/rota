#ifndef ROTA_INTERPRETER_HH
#define ROTA_INTERPRETER_HH

#include <string>

#include "vm/rotavm.hh"

class RotaInterpreter {
public:
    void run(std::string const& code);

    void print_stack() const;
    [[nodiscard]] rotavm::Value const& peek_vm() const { return vm_.peek(); }

private:
    rotavm::RotaVM vm_;
};

#endif //ROTA_INTERPRETER_HH
