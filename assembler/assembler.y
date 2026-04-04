%define api.prefix {as}

%code requires {
    typedef void *yyscan_t;
    #include <sstream>
}

%define api.pure full
%param { yyscan_t scanner }
%parse-param { std::stringstream& output }

%union {
    int i;
}

%{
#include <cstdio>
#include <string>
#include <sstream>

#include "lexer.yy.hh"

void aserror(yyscan_t scanner, std::stringstream&, const char *s);
%}

%token <i> NUMBER
%left '+' '-'
%left '*' '/'

%type <i> expr

%%

program:
      expr '\n'
    ;

expr:
      expr '+' expr  { output << "\tadd\n"; }
    | expr '-' expr  { output << "\tsub\n"; }
    | expr '*' expr  { output << "\tmul\n"; }
    | expr '/' expr  { output << "\tdiv\n"; }
    | '(' expr ')'   { $$ = $2; }
    | NUMBER         { output << "\tpush " << $1 << "\n"; }
    ;

%%

void aserror(yyscan_t scanner, std::stringstream&, const char *s) {
    fprintf(stderr, "error: %s\n", s);
}