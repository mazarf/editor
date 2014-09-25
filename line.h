#ifndef LINE_H
#define LINE_H
/* LINE struct and functions */

#include <string.h>
#include <stdlib.h>

typedef struct
{
	char *line;
	int size; // size of array, not string
} LINE;

void insert_char(LINE *s, char c, int index); // inserts char to string
void remove_char(LINE *s, int index);
void expand(LINE *s); // doubles the size of the line

#endif
