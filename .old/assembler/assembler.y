%define api.prefix {as}

%code requires {
    typedef void *yyscan_t;
    #include <cstdint>
    #include <vector>
    #include "../common/types.hh"
    #include "../common/opcodes.hh"
}

%define api.pure full
%param { yyscan_t scanner }
%parse-param { std::vector<uint8_t>& output }

%union {
    int i;
    float f;
}

%{
#include <cstdio>
#include <string>
#include <sstream>

#include "lexer.yy.hh"

void aserror(yyscan_t scanner, std::vector<uint8_t>&, const char *s);
%}

%token <i>   INTEGER
%token <f>   FLOAT
%token       PUSH ADD SUB MUL DIV HALT

%left '+' '-'
%left '*' '/'

%%

lines: lines line
     | line
     ;

line: instruction '\n';

instruction: PUSH   { output.push_back(OP_PUSH); } value
           | ADD    { output.push_back(OP_ADD);  }
           | SUB    { output.push_back(OP_SUB);  }
           | MUL    { output.push_back(OP_MUL);  }
           | DIV    { output.push_back(OP_DIV);  }
           | HALT   { output.push_back(OP_HALT); }
           ;

value: INTEGER { output.append_range(types::to_bin($1)); }
     | FLOAT   { output.append_range(types::to_bin($1)); }
     ;

%%

void aserror(yyscan_t scanner, std::vector<uint8_t>&, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}