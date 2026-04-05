%option prefix="in"
%option reentrant
%option bison-bridge
%option noyywrap

%{
#include <string>
#include "parser.tab.hh"
#define YYSTYPE INSTYPE
%}

%%

-?[0-9]*\.[0-9]+f   { yylval->f = std::stof(yytext); return FLOAT; }
-?[0-9]+            { yylval->i = atoi(yytext); return INTEGER; }
\/\/                { return DSLASH; }
[ \t]               { /* skip whitespace */ }
\n                  { return '\n'; }
.                   { return yytext[0]; }

%%