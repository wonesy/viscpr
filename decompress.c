#include "decompress.h"
#include "visual.h"


static uint8_t next_bit(struct dc_stream *s)
{
    uint8_t bit;

    bit = (s->buf & s->mask) ? 1 : 0;
    s->mask <<= 1;

    if (!s->mask) {
        s->mask = 1;

        // Read in next byte
        if (s->cur_offs < s->stream_len) {
            s->buf = s->stream[++(s->cur_offs)];
        }
    }

    return bit;
}

static uint8_t read_bits(struct dc_stream *s, int need)
{
    uint8_t bits_val = 0;

    // Save the state before reading bits
    s->prev_mask = s->mask;
    s->prev_offs = s->cur_offs;
    s->bits_requested = need;

    for (int i = 0; i < need; i++) {
        bits_val |= (next_bit(s) << i);
    }

    return bits_val;
}
/*
static void read_huff_tree(struct dc_stream *s)
{
    uint8_t hlit;
    uint8_t hdist;
    uint8_t hclen;

    hlit = read_bits(s, 5);
    hdist = read_bits(s, 5);
    hclen = read_bits(s, 4);
}
*/

/*
    Function:
        dc_build_static_tree

    From RFC 1951:

                   Lit Value    Bits        Codes
                   ---------    ----        -----
                     0 - 143     8          00110000 through
                                            10111111
                   144 - 255     9          110010000 through
                                            111111111
                   256 - 279     7          0000000 through
                                            0010111
                   280 - 287     8          11000000 through
                                            11000111

    Distance codes 0-31 are represented by (fixed-length) 5-bit codes

    Parameters:

        s - Pointer to a decompression stream

    Returns:

        None
*/
void dc_build_static_tree(struct dc_stream *s)
{
    const uint16_t NUM_LITLEN = 286;
    const uint16_t NUM_DIST = 30;

    uint8_t cl; // code length
    uint16_t symb;

    // Literal/Length tree
    for (int i = 0; i < NUM_LITLEN; i++)
    {
        if (i == 0) {
            cl = 8;
            symb = 0x30;
        } else if (i == 144) {
            cl = 9;
            symb = 0x190;
        } else if (i == 256) {
            cl = 7;
            symb = 0x0;
        } else if (i == 280) {
            cl = 8;
            symb = 0xC0;
        }

        s->tree.lit_len_cl[i] = cl;
        s->tree.lit_len_symb[i] = symb;

        symb++;
    }

    // Distance tree
    symb = 0;
    for (int i = 0; i < NUM_DIST; i++)
    {
        s->tree.dist_cl[i] = 5;
        s->tree.dist_symb[i] = symb++;
    }
}

void dc_get_bfinal(struct dc_stream *s)
{
    uint8_t bfinal;

    bfinal = read_bits(s, 1);
    s->bfinal = bfinal;
}

void dc_get_encoding(struct dc_stream *s)
{
    uint8_t encoding;

    encoding = read_bits(s, 2);
    s->encoding = encoding;
}

void dc_step(struct dc_stream *s)
{
    switch (s->cur_state)
    {
        case FSM_BFINAL:
            s->mask = 1;
            dc_get_bfinal(s);
            s->next_state = FSM_ENCODING;
            break;
        case FSM_ENCODING:
            dc_get_encoding(s);

            if (s->encoding == STATIC_ENCODING) {
                dc_build_static_tree(s);
            } else if (s->encoding == DYNAMIC_ENCODING) {
                s->next_state = FSM_DYN_HEAD_RD_HLIT;
            } else {
                s->next_state = FSM_READ_LEN_NLEN;
            }

            break;
        case FSM_BUILD_STATIC_TREE:
            break;
        default:
            break;
    }

    s->prev_state = s->cur_state;
    s->cur_state = s->next_state;
    s->next_state = FSM_IDLE;
}

struct dc_stream dc_init(uint8_t *stream, int len)
{
    struct dc_stream s;

    s.stream = stream;
    s.cur_offs = 0;
    s.bits_requested = 0;
    s.stream_len = len;
    s.buf = stream[0];
    s.mask = 0;
    s.bfinal = 0;
    s.cur_state = FSM_BFINAL;
    s.next_state = FSM_IDLE;

    s.prev_mask = s.mask;
    s.prev_state = FSM_IDLE;
    s.prev_offs = s.cur_offs;

    if (s.stream_len == 0) {
        printf("yoyoy\n");
    }

    return s;
}


