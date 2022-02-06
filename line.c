#include "line.h"


void init_line(LINE *s)
{
    s->line = malloc(LINE_SIZE * sizeof(char));
    if(s->line != NULL)
    {
        s->size = LINE_SIZE;
        s->line[0] = '\0';
    }
} //init_line  

void add_char(LINE *s, char c)
{
    insert_char(s, c, strlen(s->line));
}

// Insert char into string.
void insert_char(LINE *s, char c, int index){
    char temp[2];
    temp[0] = c;
    temp[1] = '\0';
    insert_string(s, temp, index);
} // insert char using insert_string()

void insert_string(LINE *s, char *in, int index)
{
    int line_ln = strlen(s->line);
    int in_ln = strlen(in);
    if(line_ln + in_ln >= s->size) expand(s, in_ln);
    if(line_ln + in_ln >= s->size) return;

    size_t append_ln = line_ln - index + 1;

    memmove(s->line + index + in_ln, s->line + index, append_ln  * sizeof(char));
    memcpy(s->line + index, in, (in_ln) * sizeof(char));
} // insert string

void expand(LINE *s, int ln)
{
    size_t new_size = s->size * 2 + ln;
    char *p = realloc(s->line, new_size * sizeof(char));
    if(p != NULL)
    {
        s->size = new_size;
        s->line = p;
    }
}
    
    
void remove_char(LINE *s, int index){
        size_t append_ln = strlen(s->line) - index;
        memmove(s->line + index, s->line + index + 1, append_ln * sizeof(char));
} // remove_char
