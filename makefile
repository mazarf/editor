# makefile for text.c

CC=gcc
CFLAGS=-Wall -g
OBJS=line.o page.o
LIBS=-lncurses

text:	text.c text.h $(OBJS)
	$(CC) $(CFLAGS) -o text text.c $(OBJS) $(LIBS)
	
line.o: line.c line.h
	$(CC) $(CFLAGS) -c line.c
	
page.o: page.c page.h line.h
	$(CC) $(CFLAGS) -c page.c
	
clean:
	rm -f $(OBJS) text
