#include "line.h"

void init_line(LINE *s){
    s->size = LINE_SIZE;
    s->line = (char *)malloc(LINE_SIZE * sizeof(char));
  	s->line[0] = '\0';
} // init_line

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
    size_t append_ln = line_ln - index + 1;
    size_t new_size;

    if(line_ln + in_ln >= s->size){
        new_size = s->size * 2 + in_ln;
        s->line = (char *)realloc(s->line, new_size * sizeof(char));
        s->size = new_size;    
    }
    memmove(s->line + index + in_ln, s->line + index, append_ln  * sizeof(char));
    memcpy(s->line + index, in, (in_ln) * sizeof(char));
} // insert string

void remove_char(LINE *s, int index){
        size_t append_ln = strlen(s->line) - index;
        memmove(s->line + index, s->line + index + 1, append_ln * sizeof(char));
} // remove_char
