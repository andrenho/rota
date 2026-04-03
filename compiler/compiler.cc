#include "compiler.hh"
#include "parser.tab.hh"
#include "lexer.yy.hh"

#include <sstream>

std::stringstream output;

namespace compiler {

std::string compile(std::string const& source)
{
    output.clear();
    YY_BUFFER_STATE buf = yy_scan_string(source.c_str());
    yyparse();
    yy_delete_buffer(buf);

    return output.str();
}

}