#include "vm.hh"

void VM::run(std::function<void()> before_each_instruction)
{

}

void VM::run_debug_console()
{
    run([]() {
        // TODO
    });
}
