#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main_utils.h"
#include "visual.h"

int main(int argc, char *argv[])
{
    int err = 0;

    struct viscpr_opts opts;

    err = parse_cmdln(argc, argv, &opts);
    assert(err == 0);

        
    vis_init_screen();
    vis_cleanup();

    return 0;
}
