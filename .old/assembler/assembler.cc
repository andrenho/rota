#include "assembler.hh"

#define YYSTYPE ASSTYPE
#include "assembler/parser.tab.hh"
#include "assembler/lexer.yy.hh"

#include <sstream>

namespace assembler {

std::vector<uint8_t> assemble(std::string const& source)
{
    std::vector<uint8_t> output;

    yyscan_t scanner;
    aslex_init(&scanner);
    YY_BUFFER_STATE buf = as_scan_string(source.c_str(), scanner);
    asparse(scanner, output);
    as_delete_buffer(buf, scanner);

    return output;
}

}