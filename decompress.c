#include "decompress.h"
#include "visual.h"

// Global stream information
static struct dc_stream s;

/*
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
    uint8_t bits_val;

    for (int i = 0; i < need; i++) {
        bits_val |= (next_bit(s) << i);
    }

    return bits_val;
}

static void read_huff_tree(struct dc_stream *s)
{
    uint8_t hlit;
    uint8_t hdist;
    uint8_t hclen;

    hlit = read_bits(s, 5);
    hdist = read_bits(s, 5);
    hclen = read_bits(s, 4);
}*/

void dc_step()
{
    switch (s.cur_state)
    {
        case DC_BFINAL:
            printf("here we go\n");
            break;
        default:
            printf("fuck\n");
    }
}

void dc_init(uint8_t *stream, int len)
{
    s.stream = stream;
    s.cur_offs = 0;
    s.stream_len = len;
    s.buf = 0;
    s.mask = 0;
    s.cur_state = DC_BFINAL;

    if (s.stream_len == 0) {
        printf("yoyoy\n");
    }

}


