#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "lex.h"
#include "parse.h"

AstNode *make_node(AstNodeType type, Token *token)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->type = type;
    node->token = token;
    node->children = NULL;
    node->next_sibling = NULL;
    return node;
}

void append_child(AstNode *parent, AstNode *child)
{
    AstNode *last_child = parent->children;
    if (last_child == NULL) {
        parent->children = child;
    } else {
        while (last_child->next_sibling != NULL) {
            last_child = last_child->next_sibling;
        }
        last_child->next_sibling = child;
    }
}

void print_ast(AstNode *node, int depth)
{
    for (int i = 0; i < depth; i++)
        putc(' ', stdout);
    printf("%s\n", node->token->lexeme);
    if (node->children != NULL) {
        depth += 2;
        node = node->children;
        while (node != NULL) {
            print_ast(node, depth);
            node = node->next_sibling;
        }
        depth -= 2;
    }
}

void print_ast_(AstNode *node, int depth)
{
    for (int i = 0; i < depth; i++)
        putc(' ', stdout);
    if (node->children == NULL) {
        printf("%d \"%s\"\n", node->type, node->token->lexeme);
    } else {
        printf("%d\n", node->type);
    }
    if (node->children != NULL) {
        depth += 2;
        node = node->children;
        while (node != NULL) {
            print_ast_(node, depth);
            node = node->next_sibling;
        }
        depth -= 2;
    }
}

AstNode *op(Token **tokens)
{
    AstNode *o = make_node(OP, *tokens);
    return o;
}

AstNode *number(Token **tokens)
{
    AstNode *n = make_node(NUMBER, *tokens);
    return n;
}

AstNode *factor(Token **tokens)
{
    AstNode *f = make_node(FACTOR, NULL);
    if ((*tokens)->type == NUM) {
        f->children = number(tokens);
    } else if ((*tokens)->type == OPEN_PAREN) {
        f->children = expr(tokens);
        assert((*tokens)->type == CLOSE_PAREN);
        tokens++;
    } else {
        printf("Expected number or open paren");
    }
    return f;
}

AstNode *term(Token **tokens)
{
    AstNode *t = make_node(TERM, NULL);
    AstNode *child = factor(tokens);
    tokens++;
    t->children = child;
    while ((*tokens)->type == MUL || (*tokens)->type == DIV) {
        child->next_sibling = op(tokens);
        tokens++;
        child = child->next_sibling;
        child->next_sibling = term(tokens);
        tokens++;
    }
    return t;
}

AstNode *expr(Token **tokens)
{
    AstNode *e = make_node(EXPR, NULL);
    AstNode *child = term(tokens);
    tokens++;
    e->children = child;
    while ((*tokens)->type == ADD || (*tokens)->type == SUB) {
        child->next_sibling = op(tokens);
        tokens++;
        child = child->next_sibling;
        child->next_sibling = term(tokens);
        tokens++;
    }
    return e;
}


/* AstNode *factor(Token *tokens) */
/* { */
/*     AstNode *factor = make_node(tokens); */

/*     if (tokens->type == NUM) { */

/*     } else if (tokens->type == OPEN_PAREN) { */

/*     } else { */
/*         printf("Unexpected token."); */
/*     } */
/*     return factor; */
/* } */

/* AstNode *term(Token *tokens) */
/* { */
/*     append_child(factor(tokens)); */
/* } */

/* AstNode *expr(Token *tokens) */
/* { */
/*     AstNode *parent = make_node(tokens); */
/*     term(); */
/*     while (make_node(tokens)->token.type == OP) { */
/*         op(); */
/*         term(); */
/*     } */
/* } */

/* AstNode *parse(Token *tokens) */
/* { */
/*     return expr(tokens); */
/* } */
