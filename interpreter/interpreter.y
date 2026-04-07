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

%token AND OR
%token DSLASH EQ NEQ GT_EQ LT_EQ
%token <i> INTEGER
%token <f> FLOAT
%left AND OR
%left EQ NEQ GT_EQ LT_EQ '<' '>'
%left '+' '-'
%left '*' '/' DSLASH '%'
%left '^'
%left '!'

%%

program:
      expr '\n'
    ;

expr:
      expr '+' expr     { vm.sum(); }
    | expr '-' expr     { vm.subtract(); }
    | expr '*' expr     { vm.multiply(); }
    | expr '/' expr     { vm.divide(); }
    | expr '%' expr     { vm.modulo(); }
    | expr '^' expr     { vm.power(); }
    | expr DSLASH expr  { vm.idivide(); }
    | expr EQ expr      { vm.equals(); }
    | expr NEQ expr     { vm.not_equal(); }
    | expr '>' expr     { vm.greater_than(); }
    | expr '<' expr     { vm.less_than(); }
    | expr GT_EQ expr   { vm.greater_than_or_equal(); }
    | expr LT_EQ expr   { vm.less_than_or_equal(); }
    | expr AND expr     { vm.and_(); }
    | expr OR expr      { vm.or_(); }
    | '!' expr          { vm.not_(); }
    | '(' expr ')'
    | INTEGER           { vm.push(rotavm::Value($1)); }
    | FLOAT             { vm.push(rotavm::Value($1)); }
    ;

%%

void inerror(yyscan_t scanner, rotavm::RotaVM&, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}