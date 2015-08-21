#include "prompt.h"

void prompt_string(const char *message, char *name) 
{
   echo();
   WINDOW *prompt = subwin(stdscr, PROMPT_LINES, PROMPT_COLS,
                           center_y(PROMPT_LINES), center_x(PROMPT_COLS));
   mvwprintw(prompt, 1, 1, message);
   box(prompt, 0, 0);
   wmove(prompt, PROMPT_OFFY, PROMPT_OFFX);

   wattron(prompt, A_REVERSE);
   wprintw(prompt, "                  ");
   wmove(prompt, PROMPT_OFFY, PROMPT_OFFX);
   wgetstr(prompt, name);
   wattroff(prompt, A_REVERSE);

   wborder(prompt, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
   werase(prompt);
   wrefresh(prompt);
   delwin(prompt);
   noecho();
}

int center_x(int width) 
{
    return (COLS - width) / 2;
}

int center_y(int height)
{
    return (LINES - height) / 2;
}
