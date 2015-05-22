#ifndef __VISUAL_H__
#define __VISUAL_H__

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define WIN_MARGIN 1
#define TITLE_SIZE 128

struct viscpr_win {
    WINDOW *w;
    char title[TITLE_SIZE];
    int height;
    int width;
};

void vis_init_screen();
void vis_cleanup();
void vis_init_cur_view(uint8_t *, int);

#endif
