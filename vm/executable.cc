#include "executable.hh"

namespace rotavm {

void Executable::add(OpCode opcode)
{
    functions_.at(current_function_).tokens.emplace_back(opcode);
}

void Executable::add(OpCode opcode, Value const& p1)
{
    functions_.at(current_function_).tokens.emplace_back(opcode, p1);
}

void Executable::add_function()
{
    // TODO
}

void Executable::end_function()
{
    // TODO
}

std::string Executable::debug() const
{
    std::string out;

    for (size_t i = 0; i < functions_.size(); ++i) {
        out += ".func #" + std::to_string(i) + " " + functions_.at(i).name + "\n";
        for (auto const& tok: functions_.at(i).tokens) {
            out += std::string("\t") + opcode_name(tok.opcode);
            if (tok.p1)
                out += std::string(" ") + tok.p1->debug();
            out += "\n";
        }
    }

    return out;
}

}