#ifndef LEX_H_
#define LEX_H_
#include <stdio.h>
#include <stdint.h>

#define MAX_LEXEME_LENGTH 50

typedef enum TokenType {
    SUB,
    ADD,
    DIV,
    MUL,
    OPEN_PAREN,
    CLOSE_PAREN,
    NUM
} TokenType;

typedef struct Token {
    TokenType type;
    char lexeme[MAX_LEXEME_LENGTH];
} Token;

extern const char* enum_names[];

Token **lex(FILE*);

Token *make_token(TokenType, char *);

#endif // LEX_H_
