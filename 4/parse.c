#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_TOK_LN 50

size_t bufsize = 50;
char *linebuf;

typedef enum type {
    SUB,
    ADD,
    DIV,
    MUL,
    OP,
    CP,
    NUM
} type;

typedef struct token {
    type type;
    char *lexeme;
    uint8_t *value;
} token;

typedef struct ast_node{
    char type;
    uint8_t *value;
    struct ast_node **children;
    struct ast_node *parent;
} ast_node;

token t;
int read_token();

ast_node *parse_expr(FILE *stream);

void print_tok()
{
    printf("<%s %s>\n", t.type, t.value);
}

int main() {
    linebuf = (char *)malloc(bufsize * sizeof(char));
    int r;
    while ((r = read_token()) != EOF) {
        print_tok();
    }
}

ast_node *parse_opn(FILE *stream)
{
    ast_node *o = malloc(sizeof(ast_node));
    o->type = OP;
    o->value = malloc(sizeof(char));
    *o->value = (uint8_t) '(';
    return o;
}

ast_node *parse_expr(FILE *stream)
{
    ast_node *n = malloc(sizeof(ast_node));
    read_token();
    if (strcmp(t.type, "OPN") == 0) {
        ast_node *e = malloc(sizeof(ast_node));
        e = parse_expr();
        ast_node *c = malloc(sizeof(ast_node));
        o->tag = malloc(4);
        o->tag = "CPN";
        o->lexeme = malloc(2);
        o->lexeme = ")";
    }
    return n;
}

int read_token()
{
    int result = getline(&linebuf, &bufsize, stdin);
    if (result == -1) {
        return EOF;
    }
    memcpy(t.type, &linebuf[1], 3 * sizeof(uint8_t));
    t.type[3] = '\0';
    int i = 5;
    while ((linebuf[i] != '>')) {
        t.value[i - 5] = linebuf[i];
        i++;
    }
    t.value[i-5] = '\0';
    return result;
}
