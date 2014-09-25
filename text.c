#include "text.h"

/**
 * Word Processing Program
 * Features:
 *	-Real time editing
 *	-Saving
 *	-Loading	
 **/
 
 // TODO: Create move_up, down, left, right functions

int main(int argc, char *argv[])
{
	PAGE page;
	init_page(&page);

	if(argc > 1)
	{
		load_file(argc, argv, &page);
	}
	else // initialize
	{
		page.text[0].line[0] = '\0';
		page.numlines = 1;
	}
	
	/* curses interface */
	initscr();
	noecho();
	keypad(stdscr, true);

	attron(A_REVERSE);
	mvprintw(LINES - 1, 0, "Press F4 to quit.");
	attroff(A_REVERSE);

	print_page(&page, 0, WIN_SIZE);

	int y, x;
	getyx(stdscr, y, x);
	//int yoffset = 0; // offset to account for screen scroll

	while(true)
	{
		int ch = getch();
		switch(ch)
		{
			case KEY_F(4):
				goto end;
				break;
			case KEY_F(5):
				save_file(argc, argv, &page);
				break;
			case KEY_UP:
				move_up(&page, &x, &y);
				break;
			case KEY_DOWN:
				move_down(&page, &x, &y);
				break;
			case KEY_LEFT:
				move_left(&x, &y);
				break;
			case KEY_RIGHT:
				move_right(&page, &x, &y);
				break;
			case KEY_DC:
			case 127: // backspace key...
			case KEY_BACKSPACE:
				if(page.text[y].line[x - 2] == '\0')
				{
					remove_line(&page, y);
					move_up(&page, &x, &y);
				}
				else
				{
					remove_char(&page.text[y], x - 2); // why 2?
					move_left(&x, &y);
				}
				print_page(&page, 0, WIN_SIZE);
				move(y, x);
				break;
			case '\n': // newline
				insert_line(&page, y + 1);
				print_page(&page, 0, WIN_SIZE);
				move_down(&page, &x, &y);
				break;
			default: // all other chars
				if( isprint(ch) )
				{
					insert_char(&page.text[y], ch, x - 1);
					print_page(&page, 0, WIN_SIZE);
					move_right(&page, &x, &y);
				}
		}
	}
end:	endwin();
	/* end curses */

	dest_page(&page);
	return EXIT_SUCCESS;
} // main

void move_left(int *x, int *y)
{
	if(*x - 1 > 0) move(*y, --(*x));
}


void move_right(PAGE *p, int *x, int *y)
{
	if(*x <= strlen(p->text[*y].line)) move(*y, ++(*x));
}

void move_up(PAGE *p, int *x, int *y)
{
	if( *y > 0 ) --(*y);
	if( *x > strlen(p->text[*y].line) + 1 ) // cursor adjusts
		*x = strlen(p->text[*y].line) + 1;  // to smaller lines
	move(*y, *x);
}
void move_down(PAGE *p, int *x, int *y)
{
	if( *y < WIN_SIZE ) ++(*y);
	if( *x > strlen(p->text[*y].line) + 1 )
		*x = strlen(p->text[*y].line) + 1;
	move(*y, *x);
};

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
			col++;
		}
		fputc('\n', fp);
	}
	fputc(EOF, fp);

	fclose(fp);

} // save_file
