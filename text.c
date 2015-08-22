#include "text.h"

/**
 * Word Processing Program
 * Features:
 *	-Real time editing
 *	-Saving
 *	-Loading	
 **/

int y_offset = 0; // TODO: move to local scope
int tab_offset = 0;

#define DEBUG

void print_loc(int x, int y)
{
	#ifdef DEBUG
	int oldx, oldy;
	getyx(stdscr, oldy, oldx);
	mvprintw(0, COLS - 20, "x: %d y: %d o: %d", x, y, y_offset);
	move(oldy, oldx);
	#endif
}

int main(int argc, char *argv[])
{	
	PAGE page;

	if(argc > 1)
	{
		load_file(argc, argv, &page);
	}
	else // initialize
	{
		init_page(&page, "untitled.txt", PAGE_SIZE);
		page.text[0].line[0] = '\0';
		page.numlines = 1;
	}
	
	/* curses interface */
	initscr();
	noecho();
	keypad(stdscr, true);
	
	int beg = 0;
	int end = WIN_SIZE;
	int y, x; // position on screen
	

	update_status("Press F4 to quit");
	
	print_page(&page, beg, end);
	getyx(stdscr, y, x);

    char status[NAME_LIMIT + 10];
	while(true)
	{
		beg = 0 + y_offset;
		end = WIN_SIZE + y_offset;
		int ch = getch();
		update_status("Press F4 to quit"); // default text
		switch(ch)
		{
			case KEY_F(4):
				goto endnc;
				break;
			case KEY_F(5):
				save_file(argc, argv, &page);
                sprintf(status, "Saved as \'%s\'", page.filename);
				update_status(status);
				break;
            case KEY_F(6):
                prompt_string("Save As:", page.filename);
                save_file(argc, argv, &page);
                sprintf(status, "Saved as \'%s\'", page.filename);
				print_page(&page, beg, end);
				update_status(status);
                break;
			case KEY_UP:
				move_up(&page, &x, &y);
				print_loc(x, y);
				break;
			case KEY_DOWN:
				move_down(&page, &x, &y);
				print_loc(x, y);
				break;
			case KEY_LEFT:
				move_left(&x, &y);
				print_loc(x, y);
				break;
			case KEY_RIGHT:
				move_right(&page, &x, &y);
				print_loc(x, y);
				break;
			case KEY_DC:
			case 127: // backspace key...
			case KEY_BACKSPACE:
				if(strlen(page.text[y + y_offset].line) == 0)
				{	// can only delete blank lines for now
					remove_line(&page, y + y_offset);
					move_up(&page, &x, &y);
				}
				else if( x > 1 )
				{
					remove_char(&page.text[y + y_offset], x - 2); // delete
					move_left(&x, &y);				  // char behind cursor
				}
				print_page(&page, beg, end);
				move(y, x);
				break;
			case '\n': // newline
				insert_line(&page, y + y_offset + 1);
				print_page(&page, beg, end);
				move_down(&page, &x, &y);
				break;
			default: // all other chars
				if( isprint(ch) )
				{
					insert_char(&page.text[y + y_offset], ch, x - 1);
					print_page(&page, beg, end);
					move_right(&page, &x, &y);
				}
		}
	}
endnc:	endwin();
	/* end curses */

	dest_page(&page);
	return EXIT_SUCCESS;
} // main

// prints a message at the bottom of the window
void update_status(char *info)
{
	int oldy, oldx; getyx(stdscr, oldy, oldx);
	
	attron(A_REVERSE);
	move(LINES - 1, 0);
	clrtoeol();
	printw(info);
	attroff(A_REVERSE);
	
	move(oldy, oldx);
} // update_status

/* movement */
void move_left(int *x, int *y)
{
	if(*x - 1 > 0) move(*y, --(*x));
}


void move_right(PAGE *p, int *x, int *y)
{
	if(*x <= strlen(p->text[*y + y_offset].line)) 
    {
        if(p->text[*y + y_offset].line[*x + tab_offset] == '\t') {
            move(*y, ++(*x));
        } else {
            move(*y, ++(*x));
        }
    }
}

void move_up(PAGE *p, int *x, int *y)
{
	if( *y > 0 )
	{
		--(*y);
	}
	else if (y_offset > 0)
	{
		--(y_offset);
		print_page(p, 0 + y_offset, WIN_SIZE + y_offset);	
	}
	if( *x > strlen(p->text[*y + y_offset].line) + 1 ) // cursor adjusts
		*x = strlen(p->text[*y + y_offset].line) + 1;  // to smaller lines
	move(*y, *x);
}
void move_down(PAGE *p, int *x, int *y)
{
	if( *y < WIN_SIZE - 1 && *y < p->numlines  - 1 )
	{
		 ++(*y);
	}
	else if ( *y + y_offset < p->numlines - 1 )
	{
		++(y_offset);
		print_page(p, 0 + y_offset, WIN_SIZE + y_offset);	
	}

	if( *x > strlen(p->text[*y + y_offset].line) + 1 )
		*x = strlen(p->text[*y + y_offset].line) + 1;
	move(*y, *x);
}
/* movement */

int count_lines(int argc, char **argv)
{
	FILE *fp = fopen(argv[1], "r");
	char ch = '\0';
	int count = 0;
	while((ch = fgetc(fp)) != EOF)
		if( ch == '\n' )
			count++;
	
	fclose(fp);		
	return count;
} // count_lines

/* saving and loading */
void load_file(int argc, char **argv, PAGE *p)
{
	FILE *fp = fopen(argv[1], "r");
	int size = count_lines(argc, argv) * 2;
	char ch = '\0';
	int line, col;
	init_page(p, argv[1], size);

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
	FILE *fp = fopen(p->filename, "w");
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

	fclose(fp);

} // save_file
/* saving and loading */
