#include "visual.h"
#include "visual_utils.h"

static struct viscpr_win cur_view;
static struct viscpr_win hist_view; 
//static struct viscpr_win status_view; 
//static struct viscpr_win tree_view; 

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

    // Setup hist_view
    height = LINES*0.25;
    width = COLS/2;
    strncpy(hist_view.title, "History Buffer", TITLE_SIZE);
    hist_view.w = newwin(height, width, LINES*0.75, 0);
    box(hist_view.w, 0, 0);
    wrefresh(hist_view.w);
   
    // Setup status_view
    // Setup tree_view
}

void vis_init_screen()
{
    initscr();              // Start curses mode
    cbreak();               // Disable key buffering
    keypad(stdscr, true);   // Enable arrow keys
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    vis_setup_windows();
}

void vis_cleanup()
{
    wrefresh(cur_view.w);
    delwin(cur_view.w);

    wrefresh(hist_view.w);
    delwin(hist_view.w);

    endwin();
}

void vis_init_views(uint8_t *buf, int size)
{
    util_wprint_buf(cur_view.w, cur_view.title, buf, size, 0, NO_HL_SYMB);
    util_wprint_buf(hist_view.w, hist_view.title, buf, size, 0, NO_HL_SYMB);
    wrefresh(cur_view.w);
    wrefresh(hist_view.w);
    vis_walk(buf, size);

}

void vis_walk(uint8_t *buf, int size)
{
    for (int i = 0; i < size; i++) {
        util_wprint_buf(cur_view.w, cur_view.title, buf, size, 0, i);
        wrefresh(cur_view.w);
        sleep(1);
    }
}
