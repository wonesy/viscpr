#include "visual.h"
#include "visual_utils.h"
#include "decompress.h"

static struct dc_stream s;
static struct viscpr_win cur_view;
static struct viscpr_win hist_view; 
static struct viscpr_win status_view; 
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
    height = LINES*0.5;
    width = COLS/2;
    strncpy(status_view.title, "Status", TITLE_SIZE);
    status_view.w = newwin(height, width, 0, COLS/2);
    box(status_view.w, 0, 0);
    wrefresh(status_view.w);
   
    // Setup tree_view
}

/**************************************************************************************************
 *************************************************************************************************/
static void vis_init_views(uint8_t *buf, int size)
{
    util_wprint_buf(cur_view.w, cur_view.title, buf, size, 0, NO_HL_SYMB);
    util_wprint_buf(hist_view.w, hist_view.title, buf, size, 0, NO_HL_SYMB);

    wrefresh(cur_view.w);
    wrefresh(hist_view.w);
}

/**************************************************************************************************
 *************************************************************************************************/
static void vis_init_decompress(uint8_t *buf, int len)
{
    s = dc_init(buf, len);
}

/**************************************************************************************************
 *************************************************************************************************/
int vis_start()
{
    int ret = 0;
    char step = 0;

    step = getchar();

    while (step != 'q') 
    {
        switch(s.cur_state)
        {
            case FSM_BFINAL:
                dc_get_bfinal(&s);
                break;
            default:
                ret = -1;
        }

        util_wprint_buf(cur_view.w, cur_view.title, s.stream, s.stream_len, 0, s.cur_offs);
        util_wupdate_status(status_view.w, status_view.title, s);

        step = getchar();
    }

    return ret;
}

/**************************************************************************************************
 *************************************************************************************************/
void vis_init_screen(uint8_t *buf, int len)
{
    initscr();              // Start curses mode
    cbreak();               // Disable key buffering
    keypad(stdscr, true);   // Enable arrow keys
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    vis_setup_windows();
    vis_init_views(buf, len);
    vis_init_decompress(buf, len);

    util_wupdate_status(status_view.w, status_view.title, s);
}

/**************************************************************************************************
 *************************************************************************************************/
void vis_cleanup()
{
    wrefresh(cur_view.w);
    delwin(cur_view.w);

    wrefresh(hist_view.w);
    delwin(hist_view.w);

    wrefresh(status_view.w);
    delwin(status_view.w);

    endwin();
}

/**************************************************************************************************
 *************************************************************************************************/
void vis_walk(uint8_t *buf, int size)
{
    for (int i = 0; i < size; i++) {
        util_wprint_buf(cur_view.w, cur_view.title, buf, size, 0, i);
        wrefresh(cur_view.w);
        sleep(1);
    }
}
