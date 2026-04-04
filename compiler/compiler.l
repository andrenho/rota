%option prefix="cp"
%option reentrant
%option bison-bridge
%option noyywrap

%{
#include "parser.tab.hh"
#define YYSTYPE CPSTYPE
%}

%%

[0-9]+      { yylval->i = atoi(yytext); return NUMBER; }
[ \t]       { /* skip whitespace */ }
\n          { return '\n'; }
.           { return yytext[0]; }

%%