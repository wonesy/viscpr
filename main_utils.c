#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>
#include "main_utils.h"

static char doc[] = "Visual Compression!";
static char args_doc[] = "DBGARG";

static struct argp_option argp_opts[] = {
    {"debug", 'd', 0, 0, "Print debug information", 0},
    {0}
};

static int parse_opt(int key, char *arg, struct argp_state *state)
{
    struct viscpr_opts *opts = state->input;

    switch (key)
    {
        case 'd':
            printf("Debug enabled\n");
            if (arg) printf("%s\n", arg); // just to keep gcc happy
            opts->debug = true;
            break;
        case ARGP_KEY_ARG:
            printf("cameron\n");
            break;
        case ARGP_KEY_END:
            break;
        default:
           return ARGP_ERR_UNKNOWN; 
    }

    return 0;
}

int parse_cmdln(int argc, char **argv, struct viscpr_opts *opts)
{
    static struct argp argp = {argp_opts, parse_opt, doc, args_doc, 0, 0, 0};

    return argp_parse(&argp, argc, argv, 0, 0, opts);
}
