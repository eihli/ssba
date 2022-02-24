#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parse.h"

int main()
{
    FILE *f = fopen("foo.calc", "r");
    Token *tokens = lex(f);
    while (tokens->lexeme[0] != '\0') {
        printf("%s\n", tokens->lexeme);
        tokens++;
    }
    AstNode *ast = expr(&tokens);
}
