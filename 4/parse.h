#ifndef PARSE_H_
#define PARSE_H_
#include "lex.h"

typedef enum AstNodeType {
EXPR,
TERM,
FACTOR,
OP,
NUMBER,
PAREN,
} AstNodeType;

typedef struct AstNode {
    Token token;
    struct AstNode *parent;
    struct AstNode *children;
    int child_count;
    struct AstNode *next_sibling;
} AstNode;

AstNode *make_node(Token *);
void append_child(AstNode *, AstNode *);
void print_ast(AstNode *, int);

#endif // PARSE_H_
