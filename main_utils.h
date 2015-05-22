#ifndef __MAIN_UTILS_H__
#define __MAIN_UTILS_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <argp.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_INFILE_SIZE 256

struct viscpr_opts {
    char infile[MAX_INFILE_SIZE];
    bool debug;
};

int parse_cmdln(int, char **, struct viscpr_opts *);
int rd_infile(char *, uint8_t **);
#endif
