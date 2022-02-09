#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lex.h"

#define MAX_TOKEN_LENGTH 100

char *token_type_names[7] = {"SUB", "ADD", "DIV", "MUL", "OPEN_PAREN", "CLOSE_PAREN", "NUM"};

char lexeme[MAX_LEXEME_LENGTH];

Token *make_token(TokenType type, char *lexeme)
{
    Token *token = malloc(sizeof(Token));
    token->type = type;
    strcpy(token->lexeme, lexeme);
    return token;
}

Token *lex(FILE *file)
{
    Token *tokens = malloc(MAX_TOKEN_LENGTH * sizeof(Token));
    char c;
    int token_count = 0;
    while ((c = getc(file)) != EOF) {
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
            tokens[token_count++] = *make_token(NUM, lexeme);
        } else {
            switch (c) {
                case '+':
                    tokens[token_count++] = *make_token(ADD, &c);
                    break;
                case '-':
                    tokens[token_count++] = *make_token(SUB, &c);
                    break;
                case '*':
                    tokens[token_count++] = *make_token(MUL, &c);
                    break;
                case '/':
                    tokens[token_count++] = *make_token(DIV, &c);
                    break;
                case '(':
                    tokens[token_count++] = *make_token(OPEN_PAREN, &c);
                    break;
                case ')':
                    tokens[token_count++] = *make_token(CLOSE_PAREN, &c);
                    break;
                default:
                    ;
            }
        }
    }
    Token *tail = malloc(sizeof(Token));
    *tail = (Token){0};
    tokens[token_count] = *tail;
    return tokens;
}

