%define parse.trace

%code requires {
    #include "bytecode/bytecode.hh"
    typedef void *yyscan_t;
}

%define api.pure full
%param { yyscan_t scanner }
%parse-param { Bytecode* bc }

%union {
    int   i;
    float f;
    char* s;
}

%{
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>

#include "lexer.yy.hh"

void yyerror(yyscan_t scanner, Bytecode* bc, const char *s);

%}

%token AND OR NIL FUNC RETURN
%token DSLASH EQ NEQ GT_EQ LT_EQ
%token <i> INTEGER
%token <f> FLOAT
%token <s> IDENTIFIER GLOBAL

%left AND OR
%left EQ NEQ GT_EQ LT_EQ '<' '>'
%left '+' '-'
%left '*' '/' DSLASH '%'
%right '^'
%right '!'

%start program

%%

program: 
       ;

%%

void yyerror(yyscan_t scanner, Bytecode* bc, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}
