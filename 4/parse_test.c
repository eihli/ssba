#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_TOK_LN 50
size_t bufsize = 50;

typedef enum type {
    SUB,
    ADD,
    DIV,
    MUL,
    OP,
    CP,
    NUM
} type;

const char* enum_names[] = { "SUB", "ADD", "DIV", "MUL", "OPN", "CPN", "NUM" };
const type types[] = { SUB, ADD, DIV, MUL, OP, CP, NUM };

typedef struct token {
    type type;
    char *lexeme;
    uint8_t **value;
} token;

typedef struct ast_node{
    char type;
    uint8_t **value;
    struct ast_node **children;
    struct ast_node *parent;
} ast_node;

token null_token = {0};
ast_node null_ast = {0};
token *token_buffer[1];
token *get_token(FILE *stream);
void unget_token(FILE *stream);
type str_type(char *str);

ast_node *parse_expr(FILE *stream);
ast_node *parse_opn(FILE *stream);
ast_node *parse_num(FILE *stream);
ast_node *parse_op(FILE *stream);

int node_depth(ast_node *node)
{
    int i = 0;
    while (node->parent != NULL) {
        i++;
        node = node->parent;
    }
    return i;
}

char *node_strval(ast_node *node)
{
    if (node->type == NUM) {
        char *b;
        sprintf(b, "%d", **(int **)node->value);
        return b;
    } else {
        return "+";
    }
}

void print_ast(ast_node *node)
{
    printf("1\n");
    int depth = node_depth(node);
    for (int i = 0; i < depth; i++) {
        putc('\t', stdin);
    }
    printf("%s\t%s\n", enum_names[node->type], node_strval(node));
    ast_node *child;
    for (int i = 0; i < 3; i++) {
        print_ast(child);
    }
}

int main() {
    char *linebuf;
    token_buffer[0] = &null_token;
    /* while (&token != &null_token) { */
    /*     ast_node *n = parse_opn(stdin); */
    /*     token = get_token(stdin); */
    /* } */
    ast_node *e = parse_expr(stdin);
    printf("Token: %d\n", *(int *)e->children[2]->value);
    print_ast(e);
}

ast_node *parse_expr(FILE *stream)
{
    ast_node *expr = malloc(sizeof(ast_node));
    ast_node *term1 = parse_num(stream);
    term1->parent = expr;
    ast_node *op = parse_op(stream);
    op->parent = expr;
    ast_node *term2 = parse_num(stream);
    term2->parent = expr;
    expr->value = malloc(sizeof(void *));
    *expr->value = malloc(1);
    **expr->value = (uint8_t) 9;
    expr->type = 3;
    expr->children = malloc(3 * sizeof(ast_node));
    expr->children[0] = term1;
    expr->children[1] = op;
    expr->children[2] = term2;
    return expr;
}

ast_node *parse_op(FILE *stream)
{
    token *t = get_token(stream);
    ast_node *n = malloc(sizeof(ast_node));
    n->type = t->type;
    n->value = malloc(sizeof(t->value));
    n->value = t->value;
    n->children = NULL;
    return n;
}

ast_node *parse_num(FILE *stream)
{
    token *t = get_token(stream);
    ast_node *n = malloc(sizeof(ast_node));
    n->type = t->type;
    n->value = malloc(sizeof(t->value));
    n->value = t->value;
    n->children = NULL;
    return n;
}

ast_node *parse_opn(FILE *stream)
{
    token *t = get_token(stream);
    if (t == &null_token) {
        return &null_ast;
    }
    ast_node *o = malloc(sizeof(ast_node));
    o->type = t->type;
    o->value = malloc(sizeof(char));
    **o->value = (uint8_t) '(';
    return o;
}

token peek_token(FILE *stream) {
    if (token_buffer[0] == &null_token) {
        token_buffer[0] = get_token(stream);
    }
    return *token_buffer[0];
}

type str_type(char *str) {
    for (int i = 0; i < sizeof(enum_names) / sizeof(void *); i++) {
        if (strcmp(str, enum_names[i]) == 0) {
            return types[i];
        }
    }
    return -1;
}

char *line_lexeme(char *line)
{
    int i = 5;
    char c;
    char *l = malloc(50 * sizeof(char));
    while ((c = line[i]) != '>') {
        l[i-5] = line[i];
        i++;
    }
    l[i] = '\0';
    return l;
}

void *unknown;
uint8_t **byte_value(char *lexeme, type type)
{
    if (type == NUM) {
        int *value = malloc(sizeof(int));
        *value = atoi(lexeme);
        return (uint8_t **) value;
    } else if (type == OP) {
        char *value = malloc(sizeof(char));
        *value = lexeme[0];
        return (uint8_t **) value;
    } else {
        return unknown;
    }
}

token *get_token(FILE *stream)
{
    token *t = malloc(sizeof(token));
    if (token_buffer[0] != &null_token) {
        memcpy(t, &token_buffer[0], sizeof(token));
        token_buffer[0] = &null_token;
        return t;
    } else {
        char *linebuf;
        linebuf = (char *)malloc(bufsize * sizeof(char));
        int result = getline(&linebuf, &bufsize, stream);
        if (result == -1) {
            return &null_token;
        } else {
            char type[4] = { 0 };
            memcpy(type, &linebuf[1], 3);
            t->type = str_type(type);
            t->lexeme = line_lexeme(linebuf);
            t->value = byte_value(t->lexeme, NUM);
        }
        return t;
    }
}
