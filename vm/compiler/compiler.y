%define parse.trace

%code requires {
    typedef void *yyscan_t;
    #include "vm/compiler/compiler.hh"
}

%define api.pure full
%param { yyscan_t scanner }
%parse-param { rotavm::Executable& exec }

%union {
    int   i;
    float f;
    char* s;
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
%token <s> IDENTIFIER
%left AND OR
%left EQ NEQ GT_EQ LT_EQ '<' '>'
%left '+' '-'
%left '*' '/' DSLASH '%'
%right '^'
%right '!'
%start program

%%

program: expressions    { exec.add(OpCode::Halt); }
       ;

expressions: expressions expression
           | expression
           ;

expression: '{' { exec.push_scope(); } expressions { exec.pop_scope(); } '}'
          | expr ';'                { exec.add(OpCode::Pop); }
          | RETURN expr ';'         { exec.add(OpCode::Return); }
          | IDENTIFIER '=' expr ';' { exec.assignment($1); free($1); }
          | ';'
          ;

expr: function_def
    | '!' expr              { exec.add(OpCode::Not); }
    | expr '+' expr         { exec.add(OpCode::Sum); }
    | expr '-' expr         { exec.add(OpCode::Subtract); }
    | expr '*' expr         { exec.add(OpCode::Multiply); }
    | expr '/' expr         { exec.add(OpCode::Divide); }
    | expr '%' expr         { exec.add(OpCode::Modulo); }
    | expr '^' expr         { exec.add(OpCode::Power); }
    | expr DSLASH expr      { exec.add(OpCode::IntDivide); }
    | expr EQ expr          { exec.add(OpCode::Equals); }
    | expr NEQ expr         { exec.add(OpCode::NotEqual); }
    | expr '>' expr         { exec.add(OpCode::GreaterThan); }
    | expr '<' expr         { exec.add(OpCode::LessThan); }
    | expr GT_EQ expr       { exec.add(OpCode::GreaterThanOrEqual); }
    | expr LT_EQ expr       { exec.add(OpCode::LessThanOrEqual); }
    | expr AND expr         { exec.add(OpCode::And); }
    | expr OR expr          { exec.add(OpCode::Or); }
    | '(' expr ')'
    | expr '(' ')'          { exec.add(OpCode::Call, Value(0)); }
    | IDENTIFIER            { exec.load_identifier($1); free($1); }
    | INTEGER               { exec.add(OpCode::Push, Value($1)); }
    | FLOAT                 { exec.add(OpCode::Push, Value($1)); }
    | NIL                   { exec.add(OpCode::Push, Value()); }
    ;

function_def: FUNC '(' function_parameters ')' { exec.add_function(); } '{'
                   expressions
              '}' { exec.end_function(); }
            ;

function_parameters:
                   ;

%%

void yyerror(yyscan_t scanner, rotavm::Executable&, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}