#include "visual_utils.h"

static void util_wprint_title(WINDOW *win, char *title, int startx, int starty)
{
    wmove(win, starty, startx);
    wattron(win, A_BOLD);
    wprintw(win, title);
    wattroff(win, A_BOLD);
}

/*
 *  Read the current byte from the compression stream, conv to
 *  a string containing the binary form
 */
static void util_get_byte_as_bin(char *str, struct dc_stream *s)
{
    // Assume str has space :(
    uint8_t byte;
    uint8_t mask;
    int i;

    byte = s->buf;
    mask = 0x80;    // most significant bit

    for (i = 0; i < 8; i++)
    {
        str[i] = (byte & (mask>>i) ? '1' : '0');
    }
}

/*
 * Highlight the active bits in the current byte
 */
static void hlt_cur_bits(WINDOW *win, struct dc_stream *s)
{
    char bit_string[BIT_STRING_LEN];
    uint8_t tmp_mask;

    util_get_byte_as_bin(bit_string, s);
    tmp_mask = s->prev_mask << (s->bits_requested-1);

    for (int i = 0; i < 8; i++)
    {
        if ((1<<(7-i) & tmp_mask)) {
            W_BOLD_ON;
        }

        waddch(win, bit_string[i]);

        if ((1<<(7-i) & s->prev_mask)) {
            W_BOLD_OFF;
        }
    }
}

int util_wupdate_status(WINDOW *win,  char *title, struct dc_stream *s)
{
    int x = WIN_MARGIN;
    int y = WIN_MARGIN;

    werase(win);
    box(win, 0, 0);
    util_wprint_title(win, title, WIN_MARGIN, WIN_MARGIN);

    // print the bfinal status line
    W_NEXTLN;
    IF_PREV_STATE(FSM_BFINAL, W_BOLD_ON);
    wprintw(win, "BFinal:   %s", (s->bfinal) ? "set" : "not set");
    IF_PREV_STATE(FSM_BFINAL, W_BOLD_OFF);

    // print the encoding status line
    W_NEXTLN;
    IF_PREV_STATE(FSM_ENCODING, W_BOLD_ON);
    wprintw(win, "Encoding: %s", ((s->encoding == STORED_BLOCK) ? "stored/none" :
                                  (s->encoding == DYNAMIC_ENCODING) ? "dynamic" :
                                  (s->encoding == STATIC_ENCODING) ? "static" :
                                  "unknown"));
    IF_PREV_STATE(FSM_ENCODING, W_BOLD_OFF);

    // print the mask status line
    W_NEXTLN;

    wprintw(win, "Mask: %d", s->mask);

    // print the current byte status line
    W_NEXTLN;

    wprintw(win, "Current byte: ");

    hlt_cur_bits(win, s);

    // refresh the updated window
    wrefresh(win);

    return 0;
}

/*
 *  Function:
 *
 *      util_wprint_buf
 *
 *  Description:
 *
 *      Updates the display for the history buffer, or the deflate data,
 *      highlighting the current location accordingly
 *
 *  Parameters:
 *
 *      win     = ncurses window ptr
 *      title   = title that will be displayed at the top of the window
 *      buf     = byte buffer
 *      length  = length of buf
 *      offset  = byte offset into buf where we currently are
 *      hl      = which byte to highlight
 *
 *  Return:
 *
 *      Integer: how many bytes we were able to print in the window
 */
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
            W_BOLD_ON;
        }

        wprintw(win, "%02x", buf[i]);

        // Turn off modifiers after printing char
        if (max_x > (x + 1 + WIN_MARGIN)) {
            waddch(win, ' ');
        }

        addr++;
        if (i == hl) {
            W_BOLD_OFF;
        }
    }

    wrefresh(win);

    return ret;
}

int util_wupdate_tree(WINDOW *win,  char *title, struct dc_stream *s)
{
    int x, y;
    int beg_x, beg_y;
    int max_x, max_y;
    int cur_sym_col;
    int ret = 0;

    cur_sym_col = 0;
    werase(win);
    box(win, 0, 0);

    // print the title
    util_wprint_title(win, title, WIN_MARGIN, WIN_MARGIN);

    getbegyx(win, beg_y, beg_x);
    getmaxyx(win, max_y, max_x);
    (void) beg_y; // not used, make gcc happy

    // additional space in the Y for the title
    wmove(win, WIN_MARGIN, WIN_MARGIN);

    // print huffman lit/length
    getyx(win, y, x);
    for (int i = 0; i < 285; i++) {
        // if max height is reached, try next column
        if (y + WIN_MARGIN == max_y - 1) {
            y = WIN_MARGIN;
            cur_sym_col++;
        }

        // if max width is reached, exit
        x = WIN_MARGIN + (cur_sym_col * 10);
        if (x > max_x-9) {
            ret = i;
            break;
        }

        // print trees
        wmove(win, ++y, x);
        wattron(win, COLOR_PAIR(2) | A_BOLD);
        wprintw(win, "%03d: ", i);
        wattroff(win, COLOR_PAIR(2) | A_BOLD);
        wprintw(win, "%04x", s->tree.lit_len_symb[i]);
    }

    wrefresh(win);

    return ret;
}

void util_wprint_cmd(WINDOW *win)
{
    char cmd[] = "[q] = quit; [s] = step; [p] = show next tree page";
    util_wprint_title(win, cmd, WIN_MARGIN, WIN_MARGIN);
    wrefresh(win);
}
