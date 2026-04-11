#include "executable.hh"

namespace rotavm {

void Executable::add(OpCode opcode)
{
    functions_.at(current_function_).tokens.emplace_back(opcode);
}

void Executable::add(OpCode opcode, Value const& p1, std::optional<std::string> const& var_name)
{
    functions_.at(current_function_).tokens.emplace_back(opcode, p1, var_name);
}

void Executable::add_function()
{
    functions_.at(current_function_).tokens.emplace_back(OpCode::Push, Value(rotavm::Function(current_function_ + 1)));
    auto it = functions_.emplace_back();
    it.var_idx_stack.push(0);
    ++current_function_;
}

void Executable::end_function()
{
    functions_.at(current_function_).tokens.emplace_back(OpCode::Push, Value());
    functions_.at(current_function_).tokens.emplace_back(OpCode::Return);
    current_function_ = 0;
}

void Executable::push_scope()
{
}

void Executable::pop_scope()
{
}

void Executable::assignment(std::string const& identifier)
{
    size_t var_idx;

    auto& f = functions_.at(current_function_);
    auto it = f.var_idx.find(identifier);
    if (it != f.var_idx.end()) {
        // assignment of an existing variable
        var_idx = it->second;
    } else {
        // assignment of a new variable
        var_idx = f.total_variables++;
        f.var_idx[identifier] = var_idx;
    }

    add(OpCode::StoreLocal, Value((int) var_idx), identifier);
}

void Executable::load_identifier(std::string const& identifier)
{
    auto& f = functions_.at(current_function_);
    auto it = f.var_idx.find(identifier);
    if (it != f.var_idx.end()) {
        add(OpCode::LoadLocal, Value((int) it->second), identifier);
    } else {
        throw std::runtime_error("Unknown identifier '" + identifier + "'");
    }
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
            if (tok.variable_name)
                out += std::string("  ; ") + *tok.variable_name;
            out += "\n";
        }
    }

    return out;
}

}