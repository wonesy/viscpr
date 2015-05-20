#ifndef __MAIN_UTILS_H__
#define __MAIN_UTILS_H__

struct viscpr_opts {
    bool debug;
};

int parse_cmdln(int, char **, struct viscpr_opts *);
#endif
