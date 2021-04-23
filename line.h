#ifndef LINE_H
#define LINE_H
/* LINE struct and functions */

#include <string.h>
#include <stdlib.h>

#define LINE_SIZE 128 /* Max characters in a line. Expandable */
#define TAB_WIDTH 4

typedef struct
{
	char *line;
	int size; // size of array, not string
} LINE;

void init_line(LINE *s);
void insert_char(LINE *s, char c, int index); // inserts char to string
void insert_string(LINE *s, char *str, int index);
void remove_char(LINE *s, int index);
#endif
