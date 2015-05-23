#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main_utils.h"
#include "visual.h"

int main(int argc, char *argv[])
{
    int err = 0;

    struct viscpr_opts opts;
    uint8_t *infile_buf;
    int infile_sz;

    err = parse_cmdln(argc, argv, &opts);
    assert(err == 0);
    
    infile_sz = rd_infile(opts.infile, &infile_buf);
    if (infile_sz <= 0) {
        printf("fuck\n");
    }

    vis_init_screen();
    vis_init_views(infile_buf, infile_sz);

    //dc_start(infile_buf, infile_sz);

    getchar();

    vis_cleanup();

    return 0;
}
