#include "assembler.hh"

#include "assembler/parser.tab.hh"
#include "assembler/lexer.yy.hh"

#include <sstream>

namespace assembler {

std::string assemble(std::string const& source)
{
    std::stringstream output;

    yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE buf = yy_scan_string(source.c_str(), scanner);
    yyparse(scanner, output);
    yy_delete_buffer(buf, scanner);

    return output.str();
}

}