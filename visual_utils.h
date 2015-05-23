#ifndef __VISUAL_UTILS_H__
#define __VISUAL_UTILS_H__

#include <ncurses.h>
#include <stdlib.h>
#include <stdint.h>

#define WIN_MARGIN  1
#define ADDR_SIZE   9
#define NO_HL_SYMB  -1

int util_wprint_buf(WINDOW *, char *, uint8_t *, int, int, int);
#endif
