#include "assembler.hh"

#define YYSTYPE ASSTYPE
#include "assembler/parser.tab.hh"
#include "assembler/lexer.yy.hh"

#include <sstream>

namespace assembler {

std::string assemble(std::string const& source)
{
    std::stringstream output;

    yyscan_t scanner;
    aslex_init(&scanner);
    YY_BUFFER_STATE buf = as_scan_string(source.c_str(), scanner);
    asparse(scanner, output);
    as_delete_buffer(buf, scanner);

    return output.str();
}

}