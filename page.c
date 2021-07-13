#include "page.h"

void init_page(PAGE *p, char *filename, int size)
{
    p->text = (LINE *)malloc(size * sizeof(LINE));

    int i;
    for(i = 0; i < size; i++)
    {
        init_line(p->text + i);
    }
    strcpy(p->filename, filename);
    p->numlines = 0;
    p->size = size;
} // init_page

void dest_page(PAGE *p)
{
    int i;
    for(i = 0; i < p->numlines; i++)
    {
        free(p->text[i].line); // maybe replace with dest_line()
    }
    free(p->text);
} // dest_page

// WARNING: Expansion function implemented but not tested
void insert_line(PAGE *p, int index)
{    
    size_t lines = p->numlines - index;
    size_t new_size = p->size * 2;
    if( p->numlines + 1 >= p->size ){
        p->text = (LINE *)realloc(p->text, new_size * sizeof(LINE));
        p->size = new_size;
    }
    
    memmove(&p->text[index + 1].line, &p->text[index].line, 
                lines * sizeof(LINE));
    
    LINE newline;
    init_line(&newline);
    p->text[index] = newline;
    (p->numlines)++;
} // insert_line

void remove_line(PAGE *p, int index)
{
    size_t lines;

    free(p->text[index].line);
    lines = p->numlines - index - 1; 
    memmove(&p->text[index].line, &p->text[index + 1].line,
             lines * sizeof(LINE));
    p->numlines--;
} // remove_line

// NOTE: This moves the cursor to the end of the displayed text
void print_page(const PAGE *p, int start, int end)
{
	int i, line;
	for(i = start;  i <= end; i++)
	{
		line = i - start;
		move(line, 0);
		clrtoeol();
		if(i < p->numlines)
			printw(" %s", p->text[i].line);
	}
	refresh();
} // print_page
