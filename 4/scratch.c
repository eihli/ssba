#include <stdio.h>
#include "lex.h"

int main(int argc, char *argv[])
{
    FILE *f = fopen("foo.calc", "r");
    Token **tokens = lex(f);
    while (*tokens != NULL) {
        printf("%s\n", (*tokens)->lexeme);
        tokens++;
    }
}
