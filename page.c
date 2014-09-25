#include "page.h"

void init_page(PAGE *p)
{
	p->text = (LINE *)malloc(PAGE_SIZE * sizeof(LINE));

	int i;
	for(i = 0; i < PAGE_SIZE; i++)
	{
		p->text[i].line = (char *)malloc(LINE_SIZE * sizeof(char));
		p->text[i].size = LINE_SIZE;
	}
	p->numlines = 0;
} // init_page

void dest_page(PAGE *p)
{
	int i;
	for(i = 0; i < p->numlines; i++)
	{
		free(p->text[i].line);
	}
	free(p->text);
} // dest_page



// WARNING: Will not expand once the limit of 500 lines reached
void insert_line(PAGE *p, int index)
{	
	LINE newline;
	newline.line = (char *)malloc(LINE_SIZE * sizeof(char));
	newline.line[0] = '\0';
	
	int i;

	for(i = p->numlines - 1; i >= index; i--)
		p->text[i + 1] = p->text[i];

	p->text[index] = newline;
} // insert_line



void remove_line(PAGE *p, int index)
{
	if( p->numlines > 0 )
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

void print_page(const PAGE *p, int start, int end)
{
	int i;
	for(i = start; i < p->numlines && i < end; i++)
	{
		move(i, 1);
		clrtoeol();
		printw("%s", p->text[i].line);
	}
	refresh();
}

