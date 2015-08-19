# makefile for text.c

CC=gcc
CFLAGS=-Wall -g
OBJS=text.o page.o line.o
LIBS=-lncurses

text: $(OBJS)
	$(CC) $(CFLAGS) -o text $(OBJS) $(LIBS)

text.o:	text.c text.h page.h line.h
	$(CC) $(CFLAGS) -c text.c
	
page.o: page.c page.h line.h
	$(CC) $(CFLAGS) -c page.c
	
line.o: line.c line.h
	$(CC) $(CFLAGS) -c line.c
	
clean:
	rm -f $(OBJS) text
