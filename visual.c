#include "visual.h"
#include "visual_utils.h"

static struct viscpr_win cur_view;
//static WINDOW *history_view; 

static void vis_setup_windows()
{
    int height;
    int width;

    // Setup cur_view
    height = LINES*0.75;
    width = COLS/2;
    strncpy(cur_view.title, "Current DEFLATE Position", TITLE_SIZE);
    cur_view.w = newwin(height, width, 0, 0);
    box(cur_view.w, 0, 0);
    wrefresh(cur_view.w);
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
    wrefresh(cur_view.w);
    delwin(cur_view.w);
    endwin();
}

void vis_init_cur_view(uint8_t *buf, int size)
{
    util_wprint_buf(cur_view.w, cur_view.title, buf, size, WIN_MARGIN, WIN_MARGIN);
    wrefresh(cur_view.w);
}
