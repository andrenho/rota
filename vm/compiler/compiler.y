%define parse.trace

%code requires {
    typedef void *yyscan_t;
    #include "vm/compiler/compiler.hh"
}

%define api.pure full
%param { yyscan_t scanner }
%parse-param { rotavm::Executable& exec }

%union {
    int i;
    float f;
}

%{
#include <cstdio>
#include <string>
#include <sstream>

#include "lexer.yy.hh"

using namespace rotavm;

void yyerror(yyscan_t scanner, rotavm::Executable&, const char *s);
%}

%token AND OR NIL FUNC RETURN
%token DSLASH EQ NEQ GT_EQ LT_EQ
%token <i> INTEGER
%token <f> FLOAT
%left AND OR
%left EQ NEQ GT_EQ LT_EQ '<' '>'
%left '+' '-'
%left '*' '/' DSLASH '%'
%right '^'
%right '!'
%start program

%%

program: expressions    { exec << OpCode::Halt; }
       ;

expressions: expressions expression
           | expression
           ;

expression: expr ';'          { exec << OpCode::Pop; }
          | RETURN expr ';'
          ;

expr: function_def
    | function_call
    | '!' expr          { exec << OpCode::Not; }
    | expr '+' expr     { exec << OpCode::Sum; }
    | expr '-' expr     { exec << OpCode::Subtract; }
    | expr '*' expr     { exec << OpCode::Multiply; }
    | expr '/' expr     { exec << OpCode::Divide; }
    | expr '%' expr     { exec << OpCode::Modulo; }
    | expr '^' expr     { exec << OpCode::Power; }
    | expr DSLASH expr  { exec << OpCode::IntDivide; }
    | expr EQ expr      { exec << OpCode::Equals; }
    | expr NEQ expr     { exec << OpCode::NotEqual; }
    | expr '>' expr     { exec << OpCode::GreaterThan; }
    | expr '<' expr     { exec << OpCode::LessThan; }
    | expr GT_EQ expr   { exec << OpCode::GreaterThanOrEqual; }
    | expr LT_EQ expr   { exec << OpCode::LessThanOrEqual; }
    | expr AND expr     { exec << OpCode::And; }
    | expr OR expr      { exec << OpCode::Or; }
    | '(' expr ')'
    | INTEGER           { exec << OpCode::Push << Value($1); }
    | FLOAT             { exec << OpCode::Push << Value($1); }
    | NIL               { exec << OpCode::Push << Value(); }
    ;

function_def: FUNC '(' function_parameters ')' { exec.add_function(); } '{'
                   expressions
              '}' { exec.end_function(); }
            ;

function_parameters:
                   ;

function_call: '(' ')'
             ;

%%

void yyerror(yyscan_t scanner, rotavm::Executable&, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}