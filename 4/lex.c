/* I want to write my compiler toolchain in C such that I can pipe the output from one step into the input of another step. cat foo.cal | ./scan | ./parse | ./eval. Let's not dig into why just yet. */

/* Problem is it's convenient for the parser to get "typed" tokens from the scanner. In both the solution and in Crafting Interpreters, the scanner already read the string "3.14", and it's a trivial step to store the numeric value right there too since you know it will be needed for later stages.  */

/* But it's like... whatever I choose to pipe from the scanner to the parser, the parser is going to have to know how to turn that format back into the C data structures that I need. */

/* I'm basically just taking the string  "3 * (4 + 5)" and turning it into a string "<NUM '3'> <MUL '*'> <OPEN_P '('> ...etc" which the parser has to then re-read back into C data structures. I guess my scanner normalizes whitespace and can report some errors, so my parser has a little bit of an easier time reading it. And I guess if I was pulling in external libraries I could output a structure, like XML, that already has parsing libraries written for it. */

#include <stdio.h>

#define MAX_TOK_LN  50

typedef enum {
    SUB,
    ADD,
    DIV,
    MUL,
    OP,
    CP,
    NUM
} TOK_TYPE;

const char* enum_names[] = { "SUB", "ADD", "DIV", "MUL", "OPN", "CPN", "NUM" };

typedef struct {
    TOK_TYPE tok_type;
    char value[MAX_TOK_LN];
} token;

static token t;

int next_token();
int scan_number();

void print_tok()
{
    printf("<%s %s>\n", enum_names[t.tok_type], t.value);
}

int main()
{
    int r;
    while ((r = next_token() != 0)) {
        print_tok();
    }
}

int next_token()
{
    char c = getc(stdin);
    switch (c) {
        case EOF:
            return 0;
        case '\n':
            return next_token();
        case ' ':
            return next_token();
        case '-':
            t = (token) { SUB, "-"};
            break;
        case '+':
            t = (token) { ADD, "+"};
            break;
        case '*':
            t = (token) { MUL, "*"};
            break;
        case '/':
            t = (token) { DIV, "/"};
            break;
        case '(':
            t = (token) { OP, "("};
            break;
        case ')':
            t = (token) { CP, ")"};
            break;
        default:
            if ('0' <= c && c <= '9') {
                ungetc(c, stdin);
                scan_number();
            }
    }
    return 1;
}

int is_whitespace(c) {
    return (
        c == ' '
        || c == '\t'
        || c == '\n'
    );
}

int is_digit(c) {
    return '0' <= c && c <= '9';
}

int scan_number()
{
    int i = 0;
    t.tok_type = NUM;
    while ((t.value[i] = getc(stdin)) != EOF && i < MAX_TOK_LN) {
        if (!is_digit(t.value[i])) {
            ungetc(t.value[i], stdin);
            t.value[i] = '\0';
        }
        if (t.value[i] == EOF) {
            t.value[i] = '\0';
        }
        i++;
    }
    return 1;
}
