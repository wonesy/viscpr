#ifndef __VISUAL_UTILS_H__
#define __VISUAL_UTILS_H__

#include <ncurses.h>
#include <stdlib.h>
#include <stdint.h>

void util_wprint_buf(WINDOW *, char *, uint8_t *, int , int, int);
#endif
