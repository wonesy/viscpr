#include "visual_utils.h"

static void util_wprint_title(WINDOW *win, char *title, int startx, int starty)
{
    wmove(win, starty, startx);
    wattron(win, A_BOLD);
    wprintw(win, title);
    wattroff(win, A_BOLD);
}

int util_wprint_buf(WINDOW *win, char *title, uint8_t *buf, int length, int offset, int hl)
{
    int ret;
    int addr;
    int max_x;
    int max_y;
    int beg_x;
    int beg_y;
    int x;
    int y;

    ret = length;
    addr = offset;

    util_wprint_title(win, title, WIN_MARGIN, WIN_MARGIN);

    getbegyx(win, beg_y, beg_x);
    getmaxyx(win, max_y, max_x);

    (void)beg_y;    //NOTE: unused variable

    wmove(win, WIN_MARGIN + 1, WIN_MARGIN);

    wprintw(win, "0x%06x: ", addr);
    for (int i = 0; i < length; i++) {
        getyx(win, y, x);
        
        // each "char" will take up two coordinates
        if (max_x <= (x + 2 + WIN_MARGIN)) { 
            y++;
            x = beg_x + WIN_MARGIN;
            wmove(win, y, x);

            if (max_y <= (y + WIN_MARGIN)) {
                ret = i;
                break;
            }

            wprintw(win, "0x%06x: ", addr);
        }

        if (i == hl) {
            wattron(win, COLOR_PAIR(1) | A_BOLD);
        }

        wprintw(win, "%02x", buf[i]);

        if (max_x > (x + 1 + WIN_MARGIN)) {
            waddch(win, ' ');
        }

        addr++;
        if (i == hl) {
            wattroff(win, COLOR_PAIR(1) | A_BOLD);
        }
    }

    wrefresh(win);

    return ret;
}
