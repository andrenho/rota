#include "interpreter.hh"

#include <iostream>

#define YYSTYPE INSTYPE
#include "interpreter/parser.tab.hh"
#include "interpreter/lexer.yy.hh"

void RotaInterpreter::run(std::string const& code)
{
    yyscan_t scanner;
    inlex_init(&scanner);
    YY_BUFFER_STATE buf = in_scan_string(code.c_str(), scanner);
    inparse(scanner, vm_);
    in_delete_buffer(buf, scanner);
}

void RotaInterpreter::print_stack() const
{
    std::cout << vm_.debug_stack() << "\n";
}
