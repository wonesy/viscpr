#ifndef __VISUAL_UTILS_H__
#define __VISUAL_UTILS_H__

#include <ncurses.h>
#include <stdlib.h>
#include <stdint.h>
#include "decompress.h"

#define WIN_MARGIN  1
#define ADDR_SIZE   9
#define NO_HL_SYMB  -1
#define BIT_STRING_LEN 32

#define W_NEXTLN wmove(win, ++y, x)
#define W_BOLD_ON wattron(win, COLOR_PAIR(1) | A_BOLD)
#define W_BOLD_OFF wattroff(win, COLOR_PAIR(1) | A_BOLD)

int util_wprint_buf(WINDOW *, char *, uint8_t *, int, int, int);
int util_wupdate_status(WINDOW *,  char *, struct dc_stream);

#endif
