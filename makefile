# makefile for text.c

CXX=gcc
CFLAGS=-Wall -g
OBJS=line.o page.o
LIBS=-lncurses

text:	text.c text.h $(OBJS)
	$(CXX) $(CFLAGS) -o text text.c $(OBJS) $(LIBS)
	
line.o: line.c line.h
	$(CXX) $(CFLAGS) -c line.c
	
page.o: page.c page.h line.h
	$(CXX) $(CFLAGS) -c page.c
	
clean:
	rm -f $(OBJS) text

