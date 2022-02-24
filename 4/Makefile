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

lex.o: lex.c
	$(CC) $(CFLAGS) -c $<

parse.o: parse.c
	$(CC) $(CFLAGS) -c $<

string_utils.o: string_utils.c
	$(CC) $(CFLAGS) -c $<

parse: parse.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@

test: test_copile.c lex.o parse.o
	$(CC) $(CFLAGS)

# end
