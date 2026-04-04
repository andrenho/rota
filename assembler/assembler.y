%define api.prefix {as}

%code requires {
    typedef void *yyscan_t;
    #include <cstdint>
    #include <vector>
    #include "../common/value.hh"
    #include "../common/opcodes.hh"
}

%define api.pure full
%param { yyscan_t scanner }
%parse-param { std::vector<uint8_t>& output }

%union {
    int i;
}

%{
#include <cstdio>
#include <string>
#include <sstream>

#include "lexer.yy.hh"

void aserror(yyscan_t scanner, std::vector<uint8_t>&, const char *s);
%}

%token <i>   NUMBER
%token       PUSH ADD SUB MUL DIV HALT

%left '+' '-'
%left '*' '/'

%%

lines: lines line
     | line
     ;

line: instruction '\n';

instruction: PUSH NUMBER { output.push_back(OP_PUSH); Value::Number($2).add(output); }
           | ADD         { output.push_back(OP_ADD); }
           | SUB         { output.push_back(OP_SUB); }
           | MUL         { output.push_back(OP_MUL); }
           | DIV         { output.push_back(OP_DIV); }
           | HALT        { output.push_back(OP_HALT); }
           ;

%%

void aserror(yyscan_t scanner, std::vector<uint8_t>&, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}