

#include "page.h"
void file_too_big(PAGE *p, int size);
void init_page(PAGE *p, char *filename, int size)
{
    p->text = malloc(size * sizeof(LINE));
    if(p->text == NULL) file_too_big(p, 0);
    
    int i;
    for(i = 0; i < size; i++)
    {
        init_line(p->text + i);
        if(p->text + i == NULL) file_too_big(p, i);
    }
    strcpy(p->filename, filename);
    p->numlines = size;
    p->size = size;
    

} // init_page

void dest_page(PAGE *p)
{
    int i;
    for(i = 0; i < p->numlines; i++)
    {
        free((p->text + i)->line); // maybe replace with dest_line()
    }
    free(p->text);
} // dest_page

// WARNING: Expansion function implemented but not tested
int insert_line(PAGE *p, int index)
{
    if(p->numlines + 1 >= p->size) expand_page(p);
    if(p->numlines + 1 >= p->size) return 0;
     
    size_t lines = p->numlines - index;
    memmove(&p->text[index + 1].line, &p->text[index].line, 
                lines * sizeof(LINE));
    
    LINE newline;
    init_line(&newline);
    p->text[index] = newline;
    p->numlines++;
    return 1;
} // insert_line

void remove_line(PAGE *p, int index)
{
    size_t lines;

    free(p->text[index].line);
    lines = p->numlines - index - 1; 
    memmove(&p->text[index].line, &p->text[index + 1].line,
             lines * sizeof(LINE));
    p->numlines--;
} // remove_line


void expand_page(PAGE *p)
{
    int newsize = p->size * 2 + 1;
    LINE *newline = realloc(p->text, newsize * sizeof(LINE));
    
    if(newline != NULL)
    {
        p->size = newsize;
        p->text = newline;
    }
}
        
// NOTE: This moves the cursor to the end of the displayed text
void print_page(const PAGE *p, int start, int end)
{
    int i, line;
    for(i = start;  i <= end; i++)
    {
        line = i - start;
        move(line, 0);
        clrtoeol();
        if(i < p->numlines)
            printw(" %s", p->text[i].line);
    }
    refresh();
} // print_page


void file_too_big(PAGE *p, int last_line)
{
    p->size = last_line;
    dest_page(p);
    exit(EXIT_FAILURE);
}

