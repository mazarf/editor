#include "text.h"

/**
 * Word Processing Program
 * Features:
 *    -Real time editing
 *    -Saving
 *    -Loading    
 **/

int tab_offset = 0;

#define DEBUG

void print_loc(int x, int y, int y_offset)
{
    #ifdef DEBUG
    int oldx, oldy;
    getyx(stdscr, oldy, oldx);
    mvprintw(WIN_SIZE + 1, COLS - 20, "x: %d y: %d o: %d", x, y, y_offset);
    move(oldy, oldx);
    #endif
}

int main(int argc, char *argv[])
{    
    PAGE page;

    if(argc > 1)
    {
        if(file_exists(argv[1]))
        {
            load_file(&page, argv[1]);
        }
        else
        {
            init_page(&page, argv[1], PAGE_SIZE);
            page.numlines = 1;
        }
    }
    else // initialize
    {
        init_page(&page, "untitled.txt", PAGE_SIZE);
        page.numlines = 1;
    }
    
    /* curses interface */
    initscr();
    noecho();
    keypad(stdscr, true);
    
    int beg = 0;
    int end = WIN_SIZE;
    int y, x = 0; // absolute position
    int y_draw, x_draw = 0; //position on the screen
    int y_offset = 0;
    int i;
    int old_x;
    

    update_status("Press F4 to quit");
    
    print_page(&page, beg, end);
    getyx(stdscr, y, x);

    char status[NAME_LIMIT + 10];
    while(true)
    {

        
        y = in_range(y, 0, page.numlines - 1);
        x = in_range(x, 0, strlen(page.text[y].line));

        if(y > y_offset + WIN_SIZE){
            y_offset = y - WIN_SIZE;
        } else if(y < y_offset){
            y_offset = y;
        }
        beg = 0 + y_offset;
        end = WIN_SIZE + y_offset;
        print_page(&page, beg, end);
        y_draw = y - y_offset;
        x_draw = x + 1;
        move(y_draw, x_draw);
        print_loc(x, y, y_offset);
        
        int ch = getch();
        update_status("Press F4 to quit"); // default text
        
        switch(ch)
        {
            case KEY_F(4):
                if(prompt_yesno("Are you sure you want to quit?"))
                    goto endnc;
                print_page(&page, beg, end);
                break;
            case KEY_F(5):
                save_file(&page);
                sprintf(status, "Saved: \'%s\'", page.filename);
                update_status(status);
                break;
            case KEY_F(6):
                prompt_string("Save As:", page.filename, NAME_LIMIT);
                save_file(&page);
                sprintf(status, "Saved: \'%s\'", page.filename);
                print_page(&page, beg, end);
                update_status(status);
                break;
            case KEY_UP:
                y--;
                break;
            case KEY_DOWN:
                y++;
                break;
            case KEY_LEFT:
                x--;
                break;
            case KEY_RIGHT:
                x++;
                break;
            case KEY_END:
                x = strlen(page.text[y].line);
                break;
            case KEY_HOME:
                x = 0;
                break;
            case KEY_NPAGE:
                y += 10;
                break;
            case KEY_PPAGE:
                y -= 10;
                break;
            case KEY_DC:
            case 127: // backspace key...
            case KEY_BACKSPACE:
                if(x == 0 && y != 0)
                { 
                    old_x = strlen(page.text[y - 1].line);
                    insert_string(&page.text[y - 1], page.text[y].line,
                                  strlen(page.text[y - 1].line));
                    remove_line(&page, y);
                    y--;
                    x = old_x;
                }
                else if( x > 0 )
                {
                    remove_char(&page.text[y], x - 1); // delete
                    x--;                  // char behind cursor
                }
                break;
            case '\t':
                for(i = 0; i < TAB_WIDTH; i++)
                {
                    insert_char(&page.text[y], ' ', x);
                }
                x += TAB_WIDTH;
                break;
            case '\n': // newline
                insert_line(&page, y + 1);
                insert_string(&page.text[y + 1], 
                              page.text[y].line + x, 0);
                page.text[y].line[x] = '\0';
                y++;
                x = 0;
                break;
            default: // all other chars
                if( isprint(ch) )
                {
                    insert_char(&page.text[y], ch, x);
                    x++;
                }
        }
    }
endnc:    
    /* end curses */
    endwin();
    dest_page(&page);
    return EXIT_SUCCESS;
} // main

int in_range(int integer, int floor, int ceil){
    if(integer < floor)
        return floor;
    else if(integer > ceil)
        return ceil;
    return integer;
}
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


int count_lines(FILE *fp)
{
    char ch = '\0';
    int count = 0;
    while((ch = fgetc(fp)) != EOF)
        if( ch == '\n' )
            count++;

    fseek(fp, 0, SEEK_SET); // go to beginning of file
    return count;
} // count_lines

/* saving and loading */
void load_file(PAGE *p, char *filename)
{
    FILE *fp = fopen(filename, "r");
    int size = count_lines(fp) * 2;
    char ch = '\0';
    int line;

    if(size < PAGE_SIZE)
        size = PAGE_SIZE;

    init_page(p, filename, size);

    if(fp == NULL) // file doesn't exist yet. don't bother reading
    {
        p->numlines = 1;
        return;
    }


    for(line = 0; line < size && ch != EOF; line++)
    {
        ch = fgetc(fp);
        while(ch != '\n' && ch != EOF)
        {
            LINE *currline = &(p->text[line]);
            if(ch != '\t')
            {
                insert_char(currline, ch, strlen(currline->line));
            }
            else // tab. add 4 spaces instead
            {
                int i;
                for(i = 0; i < TAB_WIDTH; i++)
                {
                    insert_char(currline, ' ', strlen(currline->line));
                }
            }
            ch = fgetc(fp);
        }
        p->numlines++;
    }

    fclose(fp);

} // load_file

void save_file(PAGE *p)
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

int file_exists(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(fp != NULL) {
        fclose(fp);
        return 1;
    }
    return 0;
}
/* saving and loading */



