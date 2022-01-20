#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "lex.h"
#include "parse.h"

AstNode *make_node(Token *token)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->token = *token;
    node->children = NULL;
    node->next_sibling = NULL;
    node->child_count = 0;
    return node;
}

void append_child(AstNode *parent, AstNode *child)
{
    AstNode *last_child = parent->children;
    while (last_child->next_sibling != NULL) {
        last_child = last_child->next_sibling;
    }
    last_child->next_sibling = child;
}

AstNode *factor(Token *tokens)
{
    AstNode *factor = make_node(tokens);

    if (tokens->type == NUM) {

    } else if (tokens->type == OPEN_PAREN) {

    } else {
        printf("Unexpected token.");
    }
    return factor;
}

AstNode *term(Token *tokens)
{
    append_child(factor(tokens));
}

AstNode *expr(Token *tokens)
{
    AstNode *parent = make_node(tokens);
    term();
    while (make_node(tokens)->token.type == OP) {
        op();
        term();
    }
}

AstNode *parse(Token *tokens)
{
    return expr(tokens);
}
