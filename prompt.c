#include "prompt.h"

static int center_x(int width);
static int center_y(int height);
static WINDOW* create_prompt(const char *message, int height, int width);
static void dest_prompt(WINDOW *prompt);

static int center_x(int width) 
{
    return (COLS - width) / 2;
}

static int center_y(int height)
{
    return (LINES - height) / 2;
}

static WINDOW* create_prompt(const char *message, int height, int width)
{
   WINDOW *prompt = subwin(stdscr, height, width,
                           center_y(height), center_x(width));
   werase(prompt);
   mvwprintw(prompt, 1, 1, message);
   wmove(prompt, 2, 0);
   whline(prompt, ACS_HLINE, 1000);
   box(prompt, 0, 0);
   wmove(prompt, PROMPT_OFFY, PROMPT_OFFX);
   return prompt;
}

static void dest_prompt(WINDOW *prompt)
{
   wborder(prompt, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
   werase(prompt);
   wrefresh(prompt);
   delwin(prompt);
}

void prompt_string(const char *message, char *name, int size) 
{
   echo();
   WINDOW *prompt = create_prompt(message, 
                                  PROMPT_STRING_LINES,
                                  PROMPT_STRING_COLS);

   wattron(prompt, A_REVERSE);
   wprintw(prompt, "                  ");
   wmove(prompt, PROMPT_OFFY, PROMPT_OFFX);
   wgetnstr(prompt, name, size);
   wattroff(prompt, A_REVERSE);

   dest_prompt(prompt);
   noecho();
}

int prompt_yesno(const char *message)
{
    int prompt_width = strlen(message) + 2;
    int yes_x = prompt_width / 2 - 8;
    int yes_y = 3;
    int no_x = prompt_width / 2 + 6;
    int no_y = 3;

    WINDOW *prompt = create_prompt(message,
                                   PROMPT_YESNO_LINES,
                                   prompt_width);

    curs_set(0); // hide cursor
    keypad(prompt, TRUE);

    int choice = 0;



    while(1)
    {
        if(choice == 0) // highlight the current choice
        {
            wattron(prompt, A_REVERSE);
            mvwprintw(prompt, yes_y, yes_x, "YES");
            wattroff(prompt, A_REVERSE);
            mvwprintw(prompt, no_y, no_x, "NO");
        }
        else
        {
            mvwprintw(prompt, yes_y, yes_x, "YES");
            wattron(prompt, A_REVERSE);
            mvwprintw(prompt, no_y, no_x, "NO");
            wattroff(prompt, A_REVERSE);
        }

        int ch = wgetch(prompt);
        switch(ch)
        {
            case KEY_LEFT:
            case KEY_RIGHT:
                choice = !choice;
                break;
            case 'y':
            case 'Y':
                choice = 0;
                goto end_prompt_yesno;
                break;
            case 'n':
            case 'N':
                choice = 1;
                goto end_prompt_yesno;
                break;
            case '\n':
                goto end_prompt_yesno;
                break;
            default:
                break;
        } 
    }

end_prompt_yesno:
    dest_prompt(prompt);
    curs_set(1);
    return !choice; // yes is 0
}

