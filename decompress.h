#ifndef __DECOMPRESS_H__
#define __DECOMPRESS_H__

#include <stdint.h>

#define STORED_BLOCK        0
#define DYNAMIC_ENCODING    1
#define STATIC_ENCODING     2

enum dc_state {
    FSM_BFINAL = 0,
    FSM_ENCODING,
};

struct dc_stream {
    uint8_t *stream;
    int cur_offs;
    int stream_len;
    uint8_t encoding;
    uint8_t bfinal;
    uint8_t buf;
    uint8_t mask;
    enum dc_state cur_state;
};

struct huff_node {
    int code;
    struct huff_node *zero;
    struct huff_note *one;
};

struct dc_stream dc_init(uint8_t *, int);
void dc_step(struct dc_stream *);
void dc_get_bfinal(struct dc_stream *);

#endif
