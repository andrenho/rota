#include "compiler.hh"

#include "compiler/parser.tab.hh"
#include "compiler/lexer.yy.hh"

void compile(std::string const& code, Bytecode* bc)
{
    yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE buf = yy_scan_string(code.c_str(), scanner);
    yyparse(scanner, bc);
    yy_delete_buffer(buf, scanner);
}