#include <ranges>

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
    auto& f = functions_.at(current_function_);
    f.variables.emplace_back();
}

void Executable::pop_scope()
{
    auto& f = functions_.at(current_function_);
    f.variables.pop_back();
}

void Executable::assignment(std::string const& identifier)
{
    auto& f = functions_.at(current_function_);
    auto var_found = f.find_variable(identifier);

    size_t var_idx;
    if (var_found) {
        // assignment of an existing variable
        var_idx = *var_found;
    } else {
        // assignment of a new variable
        var_idx = f.total_variables++;
        f.variables.back()[identifier] = var_idx;
    }

    add(OpCode::StoreLocal, Value((int) var_idx), identifier);
}

void Executable::load_identifier(std::string const& identifier)
{
    auto& f = functions_.at(current_function_);
    auto var_found = f.find_variable(identifier);
    if (var_found)
        add(OpCode::LoadLocal, Value((int) *var_found), identifier);
    else
        throw std::runtime_error("Unknown identifier '" + identifier + "'");
}

void Executable::global_assignment(std::string const& global)
{
    auto it = globals_.find(global);
    size_t global_idx;
    if (it != globals_.end()) {
        // assignment of an existing global
        global_idx = it->second;
    } else {
        // assignment of a new global
        global_idx = globals_.size();
        globals_[global] = global_idx;
    }

    add(OpCode::StoreGlobal, Value((int) global_idx), global);
}

void Executable::load_global(std::string const& global)
{
    auto it = globals_.find(global);
    if (it != globals_.end())
        add(OpCode::LoadGlobal, Value((int) it->second), global);
    else
        throw std::runtime_error("Unknown global '" + global + "'");
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

//
// FUNCTION
//

std::optional<size_t> Executable::Function::find_variable(std::string const& name) const
{
    // search variables in reverse scope

    for (auto const& variable : std::ranges::reverse_view(variables)) {
        for (auto const& [nm, pos] : variable) {
            if (name == nm)
                return pos;
        }
    }

    return {};
}

}