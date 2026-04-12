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
func                { return FUNC; }
return              { return RETURN; }
-?[0-9]*\.[0-9]+f   { yylval->f = std::stof(yytext); return FLOAT; }
-?[0-9]+            { yylval->i = atoi(yytext); return INTEGER; }
\$[A-Za-z_]+        { yylval->s = strdup(yytext); return GLOBAL; }
[A-Za-z_]+          { yylval->s = strdup(yytext); return IDENTIFIER; }
\/\/                { return DSLASH; }
@=                  { return DEBUG; }
==                  { return EQ; }
!=                  { return NEQ; }
>=                  { return GT_EQ; }
\<=                 { return LT_EQ; }
&&                  { return AND; }
\|\|                { return OR; }
[ \t\n]             { /* skip whitespace */ }
.                   { return yytext[0]; }

%%