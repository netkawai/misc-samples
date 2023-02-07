// const map
#include <stdio.h>

enum button_type {
    CLOSE, 
    MAXIMISE, 
    MINIMISE
};

struct button_type_name
{
    enum button_type key;
    char* name; 
};

static const struct button_type_name cursor_names[] = {
    {CLOSE, "Close"},
    {MAXIMISE, "Maximise"},
    {MINIMISE, "Minimise"},
};

int main()
{
    for(int i = 0 ; i < sizeof(cursor_names)/sizeof(struct button_type_name); i++)
    {
        printf("%d, %s;", cursor_names[i].key, cursor_names[i].name);
    }
}
