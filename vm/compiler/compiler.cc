#include "compiler.hh"

#include "vm/compiler/parser.tab.hh"
#include "vm/compiler/lexer.yy.hh"

namespace rotavm {

Executable compile(std::string const& code)
{
    yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE buf = yy_scan_string(code.c_str(), scanner);
    Executable exec;
    yyparse(scanner, exec);
    yy_delete_buffer(buf, scanner);
    return exec;
}

Executable& Executable::operator<<(Executable::Token const& token)
{
    functions_.at(current_function_).tokens.push_back(token);
    return *this;
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

    // TODO

    return out;
}

/*
rotavm::ExecutableFunction& ExecutableFunction::operator<<(rotavm::OpCode opcode)
{
    data_.at(current_fn_id_).push_back((uint8_t) opcode);
    return *this;
}

rotavm::ExecutableFunction& ExecutableFunction::operator<<(rotavm::Value const& value)
{
    auto& data = data_.at(current_fn_id_);

    auto dvalue = value.to_bytes();
    data.insert(data.end(), dvalue.begin(), dvalue.end());
    return *this;
}

void ExecutableFunction::add_function()
{
    FunctionId f_id = function_id_counter_++;
    *this << OpCode::Push << Function(current_fn_id_);
    current_fn_id_ = f_id;
    data_[current_fn_id_] = {};
}

void ExecutableFunction::end_function()
{
    current_fn_id_ = MAIN_FUNCTION;
}

std::vector<uint8_t> const& ExecutableFunction::data() const
{
    return data_.at(current_fn_id_);
}
 */

}