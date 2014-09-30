#ifndef LINE_H
#define LINE_H
/* LINE struct and functions */

#include <string.h>
#include <stdlib.h>

#define LINE_SIZE 128 /* Max characters in a line. Expandable */

typedef struct
{
	char *line;
	int tabs; // keeps track of tabs so we can offset
	int size; // size of array, not string
} LINE;

void init_line(LINE *s);
void insert_char(LINE *s, char c, int index); // inserts char to string
void remove_char(LINE *s, int index);
void expand(LINE *s); // doubles the size of the line

#endif
