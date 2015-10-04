#include "visual_utils.h"

static void util_wprint_title(WINDOW *win, char *title, int startx, int starty)
{
    wmove(win, starty, startx);
    wattron(win, A_BOLD);
    wprintw(win, title);
    wattroff(win, A_BOLD);
}

static void util_get_byte_as_bin(char *str, struct dc_stream s)
{
    // Assume str has space :(
    uint8_t byte;
    uint8_t mask;
    int i;

    byte = s.buf;
    mask = 0x80;    // most significant bit

    for (i = 0; i < 8; i++)
    {
        str[i] = (byte & (mask>>i) ? '1' : '0');
    }

}

int util_wupdate_status(WINDOW *win,  char *title, struct dc_stream s)
{
    char bit_string[BIT_STRING_LEN];
    int i;
    int x = WIN_MARGIN;
    int y = WIN_MARGIN;
    uint8_t tmp_mask;

    werase(win);
    box(win, 0, 0);
    util_wprint_title(win, title, WIN_MARGIN, WIN_MARGIN);

    W_NEXTLN;

    if (s.prev_state == FSM_BFINAL) {
        W_BOLD_ON;
    }

    wprintw(win, "BFinal:   %s", (s.bfinal) ? "set" : "not set");

    if (s.prev_state == FSM_BFINAL) {
        W_BOLD_OFF;
    }

    W_NEXTLN;

    if (s.prev_state == FSM_ENCODING) {
        W_BOLD_ON;
    }

    wprintw(win, "Encoding: %s", ((s.encoding == STORED_BLOCK) ? "stored/none" :
                                  (s.encoding == DYNAMIC_ENCODING) ? "dynamic" :
                                  (s.encoding == STATIC_ENCODING) ? "static" :
                                  "unknown"));
    if (s.prev_state == FSM_ENCODING) {
        W_BOLD_OFF;
    }

    W_NEXTLN;

    wprintw(win, "Mask: %d", s.mask);

    W_NEXTLN;

    util_get_byte_as_bin(bit_string, s);

    wprintw(win, "Current byte: ");

    tmp_mask = s.prev_mask << (s.bits_requested-1);

    for (i = 0; i < 8; i++)
    {
        if ((1<<(7-i) & tmp_mask)) {
            W_BOLD_ON;
        }

        waddch(win, bit_string[i]);

        if ((1<<(7-i) & s.prev_mask)) {
            W_BOLD_OFF;
        }
    }
    
    wrefresh(win);

    return 0;
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

    werase(win);
    box(win, 0, 0);

    util_wprint_title(win, title, WIN_MARGIN, WIN_MARGIN);

    getbegyx(win, beg_y, beg_x);
    getmaxyx(win, max_y, max_x);

    (void)beg_y;    //NOTE: unused variable

    // Add 1 to the y-dir due to title
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
        
        // If we've reached the highlight char, turn on modifiers
        if (i == hl) {
            wattron(win, COLOR_PAIR(1) | A_BOLD);
        }

        wprintw(win, "%02x", buf[i]);

        // Turn off modifiers after printing char
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
