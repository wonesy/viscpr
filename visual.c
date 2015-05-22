#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#include "visual.h"

static WINDOW *cur_view;
//static WINDOW *history_view; 

static void vis_setup_windows()
{
    cur_view = newwin(5, 10, 0, 0);
    box(cur_view, 0, 0);
    wrefresh(cur_view);
}

void vis_init_screen()
{
    initscr();              // Start curses mode
    raw();                  // Disable key buffering
    keypad(stdscr, true);   // Enable arrow keys

    vis_setup_windows();

    sleep(1);
}

void vis_cleanup()
{
    wrefresh(cur_view);
    delwin(cur_view);
    endwin();
}


