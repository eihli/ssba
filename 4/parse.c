#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "lex.h"
#include "parse.h"
#include "string_utils.h"

#define MAX_TOK_LN 50

size_t bufsize = 50;
char *linebuf;


typedef struct parse_result_t {
    token_type type;
    char *lexeme;
    uint8_t *value;
} parse_result_t;

typedef struct ast_node{
    parse_result_t parse_result;
    struct ast_node **children;
    struct ast_node *parent;
} ast_node;

parse_result_t parse_result;
parse_result_t parse_result_null = { -1, "NULL", 0};
parse_result_t parse_result_buf[1];

int depth = -1;
int read_token();
token_type peek_token();
char *parse_result_as_string(parse_result_t *parse_result);
void parse_expression();
void parse_term();
void parse_number();

token lookahead = {0};
int read_tok();
int match(token_type type);
void expr();
void term();
void factor();
void rest();
void term_rest();
void pad(char, int);

ast_node *parse_expr(FILE *stream);

int main()
{
    read_tok();
    expr();
}

void expr()
{
    token *t = malloc(sizeof(token));
    if (lookahead.type == NUM) {
        memcpy(t, &lookahead, sizeof(token));
    }
    term();
    pad(' ', depth);
    printf("%s\n", enum_names[lookahead.type]);
    depth++;
    pad(' ', depth);
    printf("%s\n", t->lexeme);
    rest();
    depth--;
}

void term()
{
    token *t = malloc(sizeof(token));
    if (lookahead.type == NUM) {
        memcpy(t, &lookahead, sizeof(token));
    }
    factor();
    pad(' ', depth);
    printf("%s\n", enum_names[lookahead.type]);
    depth++;
    pad(' ', depth);
    printf("%s\n", t->lexeme);
    term_rest();
    depth--;
}

void factor()
{
    switch (lookahead.type) {
        case OPEN_PAREN:
            match(OPEN_PAREN);
            expr();
            match(CLOSE_PAREN);
            break;
        case NUM:
            match(NUM);
            break;
        default:
            printf("Expected ( or NUM but got %s\n", enum_names[lookahead.type]);
    }
}

void rest()
{
    switch (lookahead.type) {
        case ADD:
        case SUB:
            match(lookahead.type);
            term();
            break;
        default:
            printf("Expected + or - but got %s\n", enum_names[lookahead.type]);
    }
}

void term_rest()
{
    switch (lookahead.type) {
        case MUL:
        case DIV:
            match(lookahead.type);
            factor();
            break;
        case OPEN_PAREN:
            expr();
            match(CLOSE_PAREN);
            break;
        default:
            ;
    }
}

int match(token_type type)
{
    if (lookahead.type != type) {
        printf("Expected %s but received %s\n", enum_names[type], enum_names[lookahead.type]);
        read_tok();
        return 0;
    }
    read_tok();
    return 1;
}


int read_tok()
{
    int l = getline(&linebuf, &bufsize, stdin);
    if (l == -1) {
        return EOF;
    }

    linebuf[l - 2] = '\0';
    linebuf++;

    split_on(' ', linebuf);
    while (*(linebuf++) != '\0')
        ;

    lookahead.type = atoi(linebuf);

    while (*(linebuf++) != '\0')
        ;
    lookahead.lexeme = linebuf;
    return 1;
}


void pad(char c, int n) {
    for (int i = 0; i < n; i++)
        putc(c, stdout);
}

void parse_expression()
{
    depth++;
    read_token();
    if (parse_result.type == OPEN_PAREN) {
        parse_expression();
        read_token(); // CLOSE_PAREN
    } else {
        parse_term();
        if (parse_result.type == ADD) {
            read_token();
            parse_term();
        } else if (parse_result.type == SUB) {
            read_token();
            parse_term();
        }
    }
    depth--;
}

void parse_term()
{
    depth++;
    parse_number();
    read_token();
    if (parse_result.type == MUL)
        printf("MUL\n");
    if (parse_result.type == DIV)
        printf("DIV\n");
    if (parse_result.type == MUL || parse_result.type == DIV) {
        read_token();
        parse_expression();
    }
    depth--;
}

void parse_number()
{
    depth++;
    char msg[50] = "NUM ";
    strcat(msg, parse_result.lexeme);
    depth--;
}

int read_token()
{
    if (parse_result_buf[0].type != parse_result_null.type) {
        parse_result = parse_result_buf[0];
        parse_result_buf[0] = parse_result_null;
        return 1;
    }
    int num_chars_read = getline(&linebuf, &bufsize, stdin);
    if (num_chars_read == -1) {
        return EOF;
    }

    // Consume the first '<' and the last '>\n'
    linebuf[num_chars_read - 2] = '\0';
    linebuf++;

    // The token format is "<type_str> <type> <lexeme>"
    split_on(' ', linebuf);
    while (*(linebuf++) != '\0')
        ;

    parse_result.type = atoi(linebuf);
    while (*(linebuf++) != '\0')
        ;

    parse_result.lexeme = linebuf;
    parse_result_buf[0] = parse_result;
    return 1;
}

token_type peek_token()
{
    if (parse_result_buf[0].type != parse_result_null.type) {
        return parse_result_buf[0].type;
    } else {
        read_token();
        return parse_result_buf[0].type;
    }
}

char *parse_result_as_string(parse_result_t *parse_result)
{
    return "Hello, world";
}
