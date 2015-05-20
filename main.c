#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#include "main_utils.h"

int main(int argc, char *argv[])
{
    struct viscpr_opts opts = {0};

    return parse_cmdln(argc, argv, &opts);
}
