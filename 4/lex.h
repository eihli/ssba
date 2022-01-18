#ifndef LEX_H_
#define LEX_H_
#include <stdint.h>

typedef enum token_type {
    SUB,
    ADD,
    DIV,
    MUL,
    OPEN_PAREN,
    CLOSE_PAREN,
    NUM,
    DECIMAL,
    STRING,
    IDENTIFIER,
} token_type;

typedef struct token {
    token_type type;
    char *lexeme;
    uint8_t *value;
} token;

// extern const char* enum_names[];

const char* enum_names[] = {
    "SUB",
    "ADD",
    "DIV",
    "MUL",
    "OPEN_PAREN",
    "CLOSE_PAREN",
    "NUM",
    "DECIMAL",
    "STRING",
    "IDENTIFIER"
};

#endif // LEX_H_
