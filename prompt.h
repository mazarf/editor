#ifndef PROMPT_H
#define PROMPT_H

#include <ncurses.h>
#include <string.h>

#define PROMPT_STRING_LINES 5
#define PROMPT_STRING_COLS 20

#define PROMPT_YESNO_LINES 5
#define PROMPT_YESNO_COLS 30

#define PROMPT_OFFX 1
#define PROMPT_OFFY 3

void prompt_string(const char *message, char *name, int size);
int prompt_yesno(const char *message);

#endif
