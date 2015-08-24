# makefile for text.c

CC=gcc
CFLAGS=-Wall -g
OBJS=text.o page.o line.o prompt.o
HEADERS=$(subst .o,.h,$(OBJS)) # text.h page.h ...
LIBS=-lncurses

text: $(OBJS)
	$(CC) $(CFLAGS) -o text $(OBJS) $(LIBS)

text.o:	text.c $(HEADERS)
	$(CC) $(CFLAGS) -c text.c
	
page.o: page.c page.h line.h
	$(CC) $(CFLAGS) -c page.c

# '$<' expands to first prerequisite file
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ 
	
clean:
	rm -f $(OBJS) text
