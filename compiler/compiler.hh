#ifndef ROTA_COMPILER_HH
#define ROTA_COMPILER_HH

#include <string>

#include "bytecode/bytecode.hh"

void compile(std::string const& code, Bytecode* bc);

#endif //ROTA_COMPILER_HH
