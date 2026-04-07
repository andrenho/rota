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

true                { return yylval->i = -1; return INTEGER; }
false               { return yylval->i = 0; return INTEGER; }
-?[0-9]*\.[0-9]+f   { yylval->f = std::stof(yytext); return FLOAT; }
-?[0-9]+            { yylval->i = atoi(yytext); return INTEGER; }
\/\/                { return DSLASH; }
==                  { return EQ; }
!=                  { return NEQ; }
>=                  { return GT_EQ; }
\<=                 { return LT_EQ; }
&&                  { return AND; }
\|\|                { return OR; }
[ \t]               { /* skip whitespace */ }
\n                  { return '\n'; }
.                   { return yytext[0]; }

%%