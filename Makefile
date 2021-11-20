TARGET=budget
CC=gcc
CFLAGS=-Wall -pedantic -D_XOPEN_SOURCE -O2

ODIR=obj

LIBS=-lm

OBJECTS = $(patsubst %.c, $(ODIR)/%.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

default: budget

budget: main.c
	$(CC) $(CFLAGS) *.c $(LIBS) -o budget

clean:
	rm -f $(ODIR)/*.o *~
