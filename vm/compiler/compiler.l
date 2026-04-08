%option reentrant
%option bison-bridge
%option noyywrap

%{
#include <string>
#include "parser.tab.hh"
%}

%%

true                { yylval->i = -1; return INTEGER; }
false               { yylval->i = 0; return INTEGER; }
nil                 { return NIL; }
-?[0-9]*\.[0-9]+f   { yylval->f = std::stof(yytext); return FLOAT; }
-?[0-9]+            { yylval->i = atoi(yytext); return INTEGER; }
\/\/                { return DSLASH; }
==                  { return EQ; }
!=                  { return NEQ; }
>=                  { return GT_EQ; }
\<=                 { return LT_EQ; }
&&                  { return AND; }
\|\|                { return OR; }
!                   { return '!'; }
[ \t]               { /* skip whitespace */ }
\n                  { return '\n'; }
.                   { return yytext[0]; }

%%