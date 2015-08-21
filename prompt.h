#ifndef PROMPT_H
#define PROMPT_H

#include <ncurses.h>
#include <string.h>

#define PROMPT_LINES 5
#define PROMPT_COLS 20

#define PROMPT_OFFX 1
#define PROMPT_OFFY 3

void prompt_string(const char *message, char *name);
int center_x(int width);
int center_y(int height);

#endif
