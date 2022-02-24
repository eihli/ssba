#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "lex.h"

int main()
{
    FILE *tmp = tmpfile();
    fputs(" 1 + (234 / 5)", tmp);
    rewind(tmp);
    Token *tokens = lex(tmp);
    assert(strcmp(tokens[0].lexeme, "1") == 0);
    assert(strcmp(tokens[1].lexeme, "+") == 0);
    assert(strcmp(tokens[2].lexeme, "(") == 0);
    assert(strcmp(tokens[3].lexeme, "234") == 0);
    assert(strcmp(tokens[4].lexeme, "/") == 0);
    assert(strcmp(tokens[5].lexeme, "5") == 0);
    assert(strcmp(tokens[6].lexeme, ")") == 0);
    assert(strcmp(tokens[7].lexeme, "") == 0);
}
