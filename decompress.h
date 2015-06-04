#ifndef __DECOMPRESS_H__
#define __DECOMPRESS_H__

#include <stdint.h>

enum dc_states {
    DC_BFINAL = 0,
    DC_ENCODING,
    DC_DYN_RD_HLIT,
    DC_DYN_RD_HDIST,
    DC_DYN_RD_HCLEN,
};


struct dc_stream {
    uint8_t *stream;
    int cur_offs;
    int stream_len;
    uint8_t buf;
    uint8_t mask;
    enum dc_states cur_state;
};

struct huff_node {
    int code;
    struct huff_node *zero;
    struct huff_note *one;
};

void dc_init(uint8_t *, int);

#endif
