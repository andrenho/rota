%define api.prefix {cp}

%code requires {
    typedef void *yyscan_t;
    #include <sstream>

    enum ValueType { T_INT, T_FLOAT };
    struct Value {
        ValueType type_;
        union {
            int i;
            float f;
        };
    };
}

%define api.pure full
%param { yyscan_t scanner }
%parse-param { std::stringstream& output }

%union {
    Value val;
}

%{
#include <cstdio>
#include <string>
#include <sstream>

#include "lexer.yy.hh"

void cperror(yyscan_t scanner, std::stringstream&, const char *s);
%}

%token <val> VALUE
%left '+' '-'
%left '*' '/'

%type_ <val> expr

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
    | VALUE          {
                        switch ($1.type_) {
                            case T_INT: output << "\tpush " << $1.i << "\n"; break;
                            case T_FLOAT: output << "\tpush " << $1.f << "f\n"; break;
                        }
                     }
    ;

%%

void cperror(yyscan_t scanner, std::stringstream&, const char *s) {
    throw std::runtime_error(std::string("error: ") + s);
}