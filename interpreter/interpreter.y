%define api.prefix {in}

%code requires {
    typedef void *yyscan_t;
    #include "vm/rotavm.hh"
}

%define api.pure full
%param { yyscan_t scanner }
%parse-param { rotavm::RotaVM& vm }

%union {
    int i;
    float f;
}

%{
#include <cstdio>
#include <string>
#include <sstream>

#include "lexer.yy.hh"

void inerror(yyscan_t scanner, rotavm::RotaVM&, const char *s);
%}

%token <i> INTEGER
%token <f> FLOAT
%left '+' '-'
%left '*' '/'

%type <i> expr

%%

program:
      expr '\n'
    ;

expr:
      expr '+' expr     { vm.sum(); }
    | expr '-' expr     { vm.subtract(); }
    | expr '*' expr     { vm.multiply(); }
    | expr '/' expr     { vm.divide(); }
    | '(' expr ')'      { $$ = $2; }
    | INTEGER             { vm.push((int) $1); }
    ;

%%

void inerror(yyscan_t scanner, rotavm::RotaVM&, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}