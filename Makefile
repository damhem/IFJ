PROJ=p
FILES=main.c parser.c string.c scanner.c stack.c generator.c linear_list.c symtable.c
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -pthread
CC=gcc
RM=rm -f

all : $(PROJ)

$(PROJ) : $(FILES)
		$(CC) $(CFLAGS) -o $(PROJ) $(FILES) -lm
