%{
#include <cstdio>
#include <string>
#include <sstream>

extern std::stringstream output;

void yyerror(const char *s);
int yylex(void);
%}

%token NUMBER
%left '+' '-'
%left '*' '/'

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

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n", s);
}