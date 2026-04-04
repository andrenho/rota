#ifndef FORTUNA_6_ASSEMBLER_HH
#define FORTUNA_6_ASSEMBLER_HH

#include <cstdint>
#include <string>
#include <vector>

namespace assembler {

std::vector<uint8_t> assemble(std::string const& source);

}

#endif
