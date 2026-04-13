%define parse.trace
%define parse.error verbose

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

Bytecode::FunctionId f_id = 0;
%}

%token AND OR NIL FUNC RETURN
%token DSLASH EQ NEQ GT_EQ LT_EQ DEBUG
%token <i> INTEGER
%token <f> FLOAT
%token <s> IDENTIFIER GLOBAL

%left AND OR
%left EQ NEQ GT_EQ LT_EQ '<' '>'
%left '+' '-'
%left '*' '/' DSLASH '%'
%right '^'
%right '!'

%start statements

%%

statements: statements statement
          | statement
          ;

statement: DEBUG expr ';'               { bc->add(f_id, Operation::SaveDebug); }
         ;

expr: INTEGER                           { bc->add(f_id, Operation::PushInt, $1); }
    ;

%%

void yyerror(yyscan_t scanner, Bytecode* bc, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}
