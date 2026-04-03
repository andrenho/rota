#include <stdio.h>

int yyparse(void);

int main(void) {
    printf("Enter expressions (Ctrl+D to quit):\n");
    return yyparse();
}