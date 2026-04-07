#include "compiler.hh"

rotavm::CompilationOutput& rotavm::CompilationOutput::operator<<(rotavm::OpCode opcode)
{
    data_.push_back((uint8_t) opcode);
    return *this;
}

rotavm::CompilationOutput& rotavm::CompilationOutput::operator<<(rotavm::Value const& value)
{
    auto dvalue = value.to_bytes();
    data_.insert(data_.end(), dvalue.begin(), dvalue.end());
    return *this;
}
