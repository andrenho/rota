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
    functions_.at(current_function_).tokens.emplace_back(OpCode::Push, Value(rotavm::Function(current_function_ + 1)));
    functions_.emplace_back();
    ++current_function_;
}

void Executable::end_function()
{
    functions_.at(current_function_).tokens.emplace_back(OpCode::Push, Value());
    functions_.at(current_function_).tokens.emplace_back(OpCode::Return);
    current_function_ = 0;
}

std::string Executable::debug() const
{
    std::string out;

    for (size_t i = 0; i < functions_.size(); ++i) {
        out += ".func fn(" + std::to_string(i) + ")\n";
        for (auto const& tok: functions_.at(i).tokens) {
            out += std::string("\t") + opcode_name(tok.opcode);
            if (tok.p1)
                out += std::string(" ") + tok.p1->debug();
            out += "\n";
        }
    }

    return out;
}

void Executable::assignment(std::string const& identifier)
{

}

void Executable::load_identifier(std::string const& identifier)
{

}

}