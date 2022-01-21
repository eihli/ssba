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
    AstNodeType type;
    Token *token;
    struct AstNode *parent;
    struct AstNode *children;
    struct AstNode *next_sibling;
} AstNode;

AstNode *make_node(AstNodeType, Token *);
void append_child(AstNode *, AstNode *);
void print_ast(AstNode *, int);
void print_ast_(AstNode *, int);

AstNode *expr(Token **);
AstNode *term(Token **);
AstNode *factor(Token **);
AstNode *number(Token **);

#endif // PARSE_H_
