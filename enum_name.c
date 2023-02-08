// const map
#include <stdio.h>


struct button_type_name
{
    enum button_type {
        CLOSE, 
        MAXIMISE, 
        MINIMISE
    } key;
    char* name; 

};

static const struct button_type_name cursor_names[] = {
    {CLOSE, "Close"},
    {MAXIMISE, "Maximise"},
    {MINIMISE, "Minimise"},
};

static const int num_of_buttons = sizeof(cursor_names)/sizeof(struct button_type_name);

struct app_button
{
    enum button_type type;
};

// NG     
// struct app_button buttons[num_of_buttons];
// OK
struct app_button *gButtons;

static void set_button_array()
{
    struct app_button buttons[num_of_buttons]; // OK
    //  NG
    // static struct app_button buttons[num_of_buttons]; 
    gButtons = &buttons[0];
}


int main()
{

    for(int i = 0 ; i < num_of_buttons; i++)
    {
        printf("%d, %s;", cursor_names[i].key, cursor_names[i].name);
    }

    // but segfault
    printf("%d",  (*(gButtons + 0)).type);
}
