#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdint.h>

#include "visual.h"

static WINDOW *cur_view;
//static WINDOW *history_view; 

static void vis_setup_windows()
{
    int height = LINES;
    int width = COLS;
    cur_view = newwin(height, width, 0, 0);
    box(cur_view, 0, 0);
    wrefresh(cur_view);
}

void vis_init_screen()
{
    initscr();              // Start curses mode
    raw();                  // Disable key buffering
    keypad(stdscr, true);   // Enable arrow keys

    vis_setup_windows();
}

void vis_cleanup()
{
    wrefresh(cur_view);
    delwin(cur_view);
    endwin();
}

void vis_init_cur_view(uint8_t *buf, int size)
{
    for (int i = 0; i < size; i++) {
        wprintw(cur_view, "%02x ", buf[i]);
    }

    wrefresh(cur_view);
}
