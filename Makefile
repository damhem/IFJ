PROJ=p
FILES=main.c parser.c string.c scanner.c stack.c exp_stack.c generator.c linear_list.c expression.c symtable.c bst.c
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -pthread
CC=gcc
RM=rm -f

all : $(PROJ)

$(PROJ) : $(FILES)
		$(CC) -g $(CFLAGS) -o $(PROJ) $(FILES) -lm
