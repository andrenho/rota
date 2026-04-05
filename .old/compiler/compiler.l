%option prefix="cp"
%option reentrant
%option bison-bridge
%option noyywrap

%{
#include <string>
#include "parser.tab.hh"
#define YYSTYPE CPSTYPE
%}

%%

[0-9]*\.[0-9]+ { yylval->val = { .type = T_FLOAT, .f = std::stof(yytext) }; return VALUE; }
[0-9]+         { yylval->val = { .type = T_INT, .i = atoi(yytext) }; return VALUE; }
[ \t]          { /* skip whitespace */ }
\n             { return '\n'; }
.              { return yytext[0]; }

%%