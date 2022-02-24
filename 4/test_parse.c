#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parse.h"

void test_tree_and_printing()
{
    Token *token_one = make_token(NUM, "1");
    Token *token_add = make_token(ADD, "+");
    Token *token_two = make_token(NUM, "2");
    Token *token_mul = make_token(MUL, "*");
    Token *token_three = make_token(NUM, "3");
    Token *tokens[5] = {token_one, token_add, token_two, token_mul, token_three};
    AstNode *expr = make_node(EXPR, NULL);
    AstNode *add = make_node(OP, tokens[1]);
    AstNode *op1 = make_node(NUMBER, tokens[0]);
    AstNode *mul = make_node(OP, tokens[3]);
    AstNode *op2 = make_node(NUMBER, tokens[2]);
    AstNode *op3 = make_node(NUMBER, tokens[4]);
    assert(strcmp(add->token->lexeme, "+") == 0);
    append_child(add, op1);
    append_child(add, mul);
    append_child(mul, op2);
    append_child(mul, op3);
    assert(strcmp((add->children)->token->lexeme, "1") == 0);
    assert(strcmp((add->children->next_sibling)->token->lexeme, "*") == 0);
    print_ast(add, 0);
}

void test_ast()
{
    Token *token_one = make_token(NUM, "1");
    Token *token_add = make_token(ADD, "+");
    Token *token_two = make_token(NUM, "2");
    Token *token_mul = make_token(MUL, "*");
    Token *token_three = make_token(NUM, "3");
    Token *token_null = malloc(sizeof(Token));
    Token *tokens[6] = {token_one, token_add, token_two, token_mul, token_three, token_null};
    print_ast_(expr(tokens), 0);
}

int main()
{
    test_tree_and_printing();
    test_ast();
    return 1;
}
