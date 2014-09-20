#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h> // -lncurses

#define PAGE_SIZE 500
#define LINE_SIZE 128
#define WIN_SIZE LINES - 2

/**
 * Word Processing Program
 * Features:
 *	-Real time editing
 *	-Saving
 *	-Loading	
 **/

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
void expand(LINE *s);
void print_page(const PAGE *p);

int main(int argc, char *argv[])
{
	PAGE page;
	init_page(&page);

	if(argc > 1)
	{
		load_file(argc, argv, &page);
	}

	/* curses interface */
	initscr();
	noecho();
	keypad(stdscr, true);

	attron(A_REVERSE);
	mvprintw(LINES - 1, 0, "Press F4 to quit.");
	attroff(A_REVERSE);

	print_page(&page);

	int y, x;
	getyx(stdscr, y, x);

	while(true)
	{
		int ch = getch();
		switch(ch)
		{
			case KEY_F(4):
				goto end;
				break;
			case KEY_UP:
				if( y >= 0 ) move(--y, x);
				break;
			case KEY_DOWN:
				if( y < WIN_SIZE ) move(++y, x);
				break;
			case KEY_LEFT:
				if(x - 1 > 0)
					move(y, --x);
				break;
			case KEY_RIGHT:
				if(x <= strlen(page.text[y].line))
					move(y, ++x);
				break;
			case KEY_DC:
			case 127: // backspace key...
			case KEY_BACKSPACE:
				remove_char(&page.text[y], x - 1);
				print_page(&page);
				move(y, --x);
				refresh();
				break;
			default: // all other chars
				if( isprint(ch) )
				{
					insert(&page.text[y], ch, x - 1);
					print_page(&page);
					move(y, ++x);
					refresh(); 
				}
		}
	}
end:	endwin();
	/* end curses */

	dest_page(&page);
	return EXIT_SUCCESS;
} // main


void load_file(int argc, char **argv, PAGE *p)
{
	FILE *fp = fopen(argv[1], "r");
	char ch = 0;
	int line, col;

	for(line = 0; line < PAGE_SIZE && ch != EOF; line++)
	{
		for(col = 0; col < LINE_SIZE - 1 && ((ch = fgetc(fp)) != '\n') && ch != EOF; col++)
		{
			p->text[line].line[col] = ch;
		}
		p->text[line].line[col] = '\0';
		p->numlines++;
	}

	fclose(fp);

} // load_file

void save_file(int argc, char **argv, PAGE *p)
{
	FILE *fp = fopen("save.txt", "w");
	int line, col;

	for(line = 0; line < p->numlines; line++)
	{
		col = 0;
		while(p->text[line].line[col] != '\0')
		{
			fputc(p->text[line].line[col], fp);
		}
		fputc('\n', fp);
	}
	fputc(EOF, fp);

	fclose(fp);

} // save_file

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

// Insert char into string. 
void insert(LINE *s, char c, int index)
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
} // remove

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

void print_page(const PAGE *p)
{
	int i;
	for(i = 0; i < p->numlines && i < WIN_SIZE; i++)
		mvprintw(i, 1, "%s ", p->text[i].line);
}
