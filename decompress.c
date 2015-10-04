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
            printf("here we go\n");
            break;
        default:
            printf("fuck\n");
    }
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


