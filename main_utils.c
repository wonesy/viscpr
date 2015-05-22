#include "main_utils.h"

static char doc[] = "Visual Compression!";
static char args_doc[] = "DBGARG";

static struct argp_option argp_opts[] = {
    {"debug", 'd', 0, 0, "Print debug information", 0},
    {"infile", 'i', 0, 0, "Input file", 0},
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
        case 'i':
            printf("Using input file %s\n", arg);
            strncpy(opts->infile, arg, MAX_INFILE_SIZE);
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

static int get_file_size(int fileno)
{
    struct stat st;

    if (fstat(fileno, &st)) {
        printf("Error getting stat information of fd %d\n", fileno);
        errno = ENOENT;
        return -1;
    }

    return st.st_size;
}

int rd_infile(char *infile, void *buf)
{
    int size;
    FILE *infile_fp;

    infile_fp = fopen(infile, "r");
    if (!infile_fp) {
        printf("Failed to open file %s", infile);
        errno = ENOENT;
        return -1;
    }

    size = get_file_size(fileno(infile_fp));
    assert(size > 0);

    buf = malloc(size);

    return fread(buf, 1, size, infile_fp); 
}
