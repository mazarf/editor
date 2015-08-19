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
	if( p->numlines >= p->size ) expand_page(p);
	
	LINE newline;
	init_line(&newline);
	newline.line[0] = '\0';
	
	int i;

	for(i = p->numlines - 1; i >= index; i--)
		p->text[i + 1] = p->text[i];

	p->text[index] = newline;
	(p->numlines)++;
} // insert_line



void remove_line(PAGE *p, int index)
{
	if( p->numlines > 1 )
	{
		free(p->text[index].line);
	
		int i;
		for(i = index; i < p->numlines - 1; i++)
		{
			p->text[i] = p->text[i + 1];
		}
		(p->numlines)--;
	}
} // remove_line

void expand_page(PAGE *p)
{
	int newsize = p->size * 2;
	LINE *newline = malloc(newsize * sizeof(LINE));
	
	int i;
	for(i = 0; i < p->size; i++) // copy old lines
		newline[i] = p->text[i];
	for(i = p->size; i < newsize; i++) // init new lines
		init_line(newline + i);
		
	p->text = newline;
	p->size = newsize;
} // expand_page

// NOTE: This moves the cursor to the end of the displayed text
void print_page(const PAGE *p, int start, int end)
{
	int i, line;
	for(i = start, line = 0; i < p->numlines && i < end; i++, line++)
	{
		move(line, 0);
		clrtoeol();
		printw(" %s", p->text[i].line);
	}
    if(start < end)
    {
        move(line, 0);   
        clrtoeol(); // if we deleted a line this may be necessary
        move(line-1, 1);
    }
	refresh();
} // print_page

