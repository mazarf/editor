#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h> // -lncurses

#define PAGE_SIZE 500
#define LINE_SIZE 128
#define WIN_SIZE LINES - 2

typedef struct
{
	char *line;
	int size; 
} LINE;

typedef struct
{
	LINE *text; // lines of text
	int numlines;
} PAGE;

void load_file(int argc, char **argv, PAGE *p);
void save_file(int argc, char **argv, PAGE *p);
void init_page(PAGE *p);
void dest_page(PAGE *p);
void insert(LINE *s, char c, int index); // inserts to string
void remove_char(LINE *s, int index);
void remove_line(PAGE *p, int index);
void expand(LINE *s);
void print_page(const PAGE *p, int start, int end);

#endif
