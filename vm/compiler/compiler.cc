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

}