#include "line.h"

void init_line(LINE *s)
{
	s->size = LINE_SIZE;
	s->line = (char *)malloc(LINE_SIZE * sizeof(char));
} // init_line

// Insert char into string. 
void insert_char(LINE *s, char c, int index)
{
	int i;

	if(strlen(s->line) >= s->size - 2) expand(s);

	for(i = strlen(s->line); i >= index; i--)
		s->line[i + 1] = s->line[i];

	s->line[index] = c;
} // insert



void remove_char(LINE *s, int index)
{
		int i;
		int len = strlen(s->line);
		for(i = index; i < len; i++)
			s->line[i] = s->line[i + 1];
} // remove_char



// expands size of line
void expand(LINE *s)
{
	int new_size = s->size * 2;
	char *temp = (char *)malloc(new_size * sizeof(char));
	strcpy(temp, s->line);
	free(s->line);
	s->line = temp;
	s->size = new_size;	
} // expand
