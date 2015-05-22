#include "visual_utils.h"

static void util_wprint_title(WINDOW *win, char *title, int startx, int starty)
{
    wmove(win, starty, startx);
    wattron(win, A_BOLD);
    wprintw(win, title);
    wattroff(win, A_BOLD);
}

void util_wprint_buf(WINDOW *win, char *title, uint8_t *buf, int size, int startx, int starty)
{
    const int margin = 1;

    int max_x;
    int max_y;
    int beg_x;
    int beg_y;
    int x;
    int y;

    util_wprint_title(win, title, startx, starty);

    getbegyx(win, beg_y, beg_x);
    getmaxyx(win, max_y, max_x);

    wmove(win, starty + 1, startx);

    for (int i = 0; i < size; i++) {
        getyx(win, y, x);
        
        // each "char" will take up two coordinates
        if (max_x <= (x + 2 + margin)) { 
            y++;
            x = beg_x + margin;
            wmove(win, y, x);
        }

        if (max_y <= (y + margin)) {
            printf("y too long whatever\n");
        }

        if (beg_y != 0) printf("damn gcc!\n");

        wprintw(win, "%02x", buf[i]);

        if (max_x > (x + 1 + margin)) {
            waddch(win, ' ');
        }
    }

    wrefresh(win);
}
