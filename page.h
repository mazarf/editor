#ifndef PAGE_H
#define PAGE_H
/* PAGE struct definition and related functions */

#include <ncurses.h> // might have to move this
#include "line.h" 

#define PAGE_SIZE 500 /* Default number of lines */
#define WIN_SIZE (LINES - 2) /* Size of window, making room for bottom prompt */
#define NAME_LIMIT 256 /* Max size of a unix filename + 1 */

typedef struct
{
    char filename[NAME_LIMIT];
	LINE *text; // lines of text
	int numlines;
	int size; // size of array
} PAGE;

void init_page(PAGE *p, char *filename, int size);
void dest_page(PAGE *p);
void insert_line(PAGE *p, int index);
void remove_line(PAGE *p, int index);
void expand_page(PAGE *p);
void print_page(const PAGE *p, int start, int end);

#endif
