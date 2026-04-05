#include "compiler.hh"

#define YYSTYPE CPSTYPE
#include "compiler/parser.tab.hh"
#include "compiler/lexer.yy.hh"

#include <sstream>

namespace compiler {

std::string compile(std::vector<std::string> const& sources)
{
    std::stringstream output;

    for (auto const& source: sources) {
        yyscan_t scanner;
        cplex_init(&scanner);
        YY_BUFFER_STATE buf = cp_scan_string(source.c_str(), scanner);
        cpparse(scanner, output);
        cp_delete_buffer(buf, scanner);
    }

    output << "\thalt\n";
    return output.str();
}

}