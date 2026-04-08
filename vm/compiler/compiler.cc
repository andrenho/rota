#include "compiler.hh"

#include "vm/compiler/parser.tab.hh"
#include "vm/compiler/lexer.yy.hh"

namespace rotavm {

CompilationOutput::CompilationOutput()
{
}

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
    data_.push_back((uint8_t) opcode);
    return *this;
}

rotavm::CompilationOutput& CompilationOutput::operator<<(rotavm::Value const& value)
{
    auto dvalue = value.to_bytes();
    data_.insert(data_.end(), dvalue.begin(), dvalue.end());
    return *this;
}

}