#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lex.h"

#define INITIAL_TOKEN_LEN 1

char *token_type_names[7] = {"SUB", "ADD", "DIV", "MUL", "OPEN_PAREN", "CLOSE_PAREN", "NUM"};

char lexeme[MAX_LEXEME_LENGTH];
int token_count = 0;

Token *make_token(TokenType type, char *lexeme)
{
    Token *token = malloc(sizeof(Token));
    token_count++;
    token->type = type;
    strcpy(token->lexeme, lexeme);
    return token;
}

Token **lex(FILE *file)
{
    int size = INITIAL_TOKEN_LEN;
    Token **head, **tokens = malloc(size * sizeof(void *));
    head = tokens;
    char c;
    while ((c = getc(file)) != EOF) {
        if (token_count == size) {
            printf("Bumping size\n");
            size *= 2;
            *head = *tokens = realloc(head, size);
            *tokens += size / 2;
        }
        if (isblank(c)) {
            continue;
        } else if (isdigit(c)) {
            int i = 0;
            lexeme[i++] = c;
            while (isdigit(c = getc(file))) {
                lexeme[i++] = c;
            }
            lexeme[i] = '\0';
            ungetc(c, file);
            *tokens++ = make_token(NUM, lexeme);
        } else {
            switch (c) {
                case '+':
                    *tokens++ = make_token(ADD, &c);
                    break;
                case '-':
                    *tokens++ = make_token(SUB, &c);
                    break;
                case '*':
                    *tokens++ = make_token(MUL, &c);
                    break;
                case '/':
                    *tokens++ = make_token(DIV, &c);
                    break;
                case '(':
                    *tokens++ = make_token(OPEN_PAREN, &c);
                    break;
                case ')':
                    *tokens++ = make_token(CLOSE_PAREN, &c);
                    break;
                default:
                    ;
            }
        }
    }
    *tokens = NULL;
    return head;
}

