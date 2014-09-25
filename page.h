#ifndef PAGE_H
#define PAGE_H
/* PAGE struct definition and related functions */

#include <ncurses.h> // might have to move this
#include "line.h" 

#define PAGE_SIZE 500 /* Max number of lines. Currently not expandable. */
#define LINE_SIZE 128 /* Max characters in a line. Expandable */
#define WIN_SIZE LINES - 2 /* Size of window, making room for bottom prompt */

typedef struct
{
	LINE *text; // lines of text
	int numlines;
} PAGE;

void init_page(PAGE *p);
void dest_page(PAGE *p);
void insert_line(PAGE *p, int index);
void remove_line(PAGE *p, int index);
void print_page(const PAGE *p, int start, int end);

#endif
