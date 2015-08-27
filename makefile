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
# NOTE: this rule is already implicit
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY: cleanall clean cleantxt
cleanall: clean cleantxt

clean:
	rm -f $(OBJS) text

cleantxt:
	rm -f *.txt
