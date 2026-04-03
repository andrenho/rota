%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);
%}

%token NUMBER
%left '+' '-'
%left '*' '/'

%%

program:
      program expr '\n'
    | /* empty */
    ;

expr:
      expr '+' expr  { printf("\tadd\n"); }
    | expr '-' expr  { printf("\tsub\n"); }
    | expr '*' expr  { printf("\tmul\n"); }
    | expr '/' expr  { printf("\tdiv\n"); }
    | '(' expr ')'   { $$ = $2; }
    | NUMBER         { printf("\tpush %d\n", $1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n", s);
}