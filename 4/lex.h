#ifndef LEX_H_
#define LEX_H_

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

extern const char* enum_names[];


#endif // LEX_H_
