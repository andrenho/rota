%define parse.trace

%code requires {
    typedef void *yyscan_t;
    #include "vm/compiler/compiler.hh"
}

%define api.pure full
%param { yyscan_t scanner }
%parse-param { rotavm::CompilationOutput& cc }

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

void yyerror(yyscan_t scanner, rotavm::CompilationOutput&, const char *s);
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

program: expressions    { cc << OpCode::Halt; }
       ;

expressions: expressions expression
           | expression
           ;

expression: expr ';'          { cc << OpCode::Pop; }
          | RETURN expr ';'
          ;

expr: function_def
    | function_call
    | '!' expr          { cc << OpCode::Not; }
    | expr '+' expr     { cc << OpCode::Sum; }
    | expr '-' expr     { cc << OpCode::Subtract; }
    | expr '*' expr     { cc << OpCode::Multiply; }
    | expr '/' expr     { cc << OpCode::Divide; }
    | expr '%' expr     { cc << OpCode::Modulo; }
    | expr '^' expr     { cc << OpCode::Power; }
    | expr DSLASH expr  { cc << OpCode::IntDivide; }
    | expr EQ expr      { cc << OpCode::Equals; }
    | expr NEQ expr     { cc << OpCode::NotEqual; }
    | expr '>' expr     { cc << OpCode::GreaterThan; }
    | expr '<' expr     { cc << OpCode::LessThan; }
    | expr GT_EQ expr   { cc << OpCode::GreaterThanOrEqual; }
    | expr LT_EQ expr   { cc << OpCode::LessThanOrEqual; }
    | expr AND expr     { cc << OpCode::And; }
    | expr OR expr      { cc << OpCode::Or; }
    | '(' expr ')'
    | INTEGER           { cc << OpCode::Push << $1; }
    | FLOAT             { cc << OpCode::Push << $1; }
    | NIL               { cc << OpCode::Push << Value(); }
    ;

function_def: FUNC '(' ')' '{' expressions '}'
            ;

function_call: '(' ')'
             ;

%%

void yyerror(yyscan_t scanner, rotavm::CompilationOutput&, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}