#include "compiler.hh"

#include "vm/compiler/parser.tab.hh"
#include "vm/compiler/lexer.yy.hh"

namespace rotavm {

std::vector<uint8_t> compile(std::string const& code)
{
    yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE buf = yy_scan_string(code.c_str(), scanner);
    CompilationOutput cc;
    yyparse(scanner, cc);
    yy_delete_buffer(buf, scanner);
    return cc.data();
}

rotavm::CompilationOutput& CompilationOutput::operator<<(rotavm::OpCode opcode)
{
    data_.at(current_fn_id_).push_back((uint8_t) opcode);
    return *this;
}

rotavm::CompilationOutput& CompilationOutput::operator<<(rotavm::Value const& value)
{
    auto& data = data_.at(current_fn_id_);

    auto dvalue = value.to_bytes();
    data.insert(data.end(), dvalue.begin(), dvalue.end());
    return *this;
}

void CompilationOutput::add_function()
{
    FunctionId f_id = function_id_counter_++;
    *this << OpCode::Push << Function(current_fn_id_);
    current_fn_id_ = f_id;
    data_[current_fn_id_] = {};
}

void CompilationOutput::end_function()
{
    current_fn_id_ = MAIN_FUNCTION;
}

std::vector<uint8_t> const& CompilationOutput::data() const
{
    return data_.at(current_fn_id_);
}

}