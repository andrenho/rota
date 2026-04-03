%define api.pure full
%param { yyscan_t scanner }

%union {
    int i;
}

%{
#include <cstdio>
#include <string>
#include <sstream>

extern std::stringstream output;

#include "lexer.yy.hh"

void yyerror(yyscan_t scanner, const char *s);
%}

%code requires {
    typedef void *yyscan_t;
}

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

void yyerror(yyscan_t scanner, const char *s) {
    fprintf(stderr, "error: %s\n", s);
}