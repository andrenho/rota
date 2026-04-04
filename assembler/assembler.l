%option prefix="as"
%option reentrant
%option bison-bridge
%option noyywrap

%{
#include "parser.tab.hh"
#define YYSTYPE ASSTYPE
%}

%%

"push"      { return PUSH; }
"add"       { return ADD; }
"sub"       { return SUB; }
"mul"       { return MUL; }
"div"       { return DIV; }
"halt"      { return HALT; }
[0-9]+      { yylval->i = atoi(yytext); return NUMBER; }
[ \t]       { /* skip whitespace */ }
\n          { return '\n'; }
.           { return yytext[0]; }

%%