#include "compiler.hh"

#include "parser.tab.hh"
#include "lexer.yy.hh"

#include <sstream>

std::stringstream output;

namespace compiler {

std::string compile(std::string const& source)
{
    output.clear();

    yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE buf = yy_scan_string(source.c_str(), scanner);
    yyparse(scanner);
    yy_delete_buffer(buf, scanner);

    return output.str();
}

}