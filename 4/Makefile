##
# Calculator
#
# @file
# @version 0.1

CC = clang
CFLAGS = -v -g -Wall -Werror
OBJS = string_utils.o

string_utils: string_utils.c
	$(CC) $(CFLAGS) $< -o $@

lex: lex.c
	$(CC) $(CFLAGS) $< -o $@

parse: parse.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@

# end
