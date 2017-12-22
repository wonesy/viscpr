#ifndef __DECOMPRESS_H__
#define __DECOMPRESS_H__

#include <stdint.h>

#define STORED_BLOCK        0
#define STATIC_ENCODING     1
#define DYNAMIC_ENCODING    2

enum dc_state {
    FSM_IDLE = 0,
    FSM_BFINAL,
    FSM_ENCODING,
    FSM_BUILD_STATIC_TREE,
    FSM_DYN_HEAD_RD_HLIT,
    FSM_DYN_HEAD_RD_HDIST,
    FSM_DYN_HEAD_RD_HCLEN,
    FSM_READ_LEN_NLEN
};

struct huff_node {
    int code;
    struct huff_node *zero;
    struct huff_node *one;
};

struct huff_tree {
    uint8_t lit_len_cl[286];
    uint8_t dist_cl[30];

    uint16_t lit_len_symb[286];
    uint16_t dist_symb[30];
};

struct dc_stream {
    uint8_t *stream;            // input data stream
    int cur_offs;               // offset in stream for coloring
    int prev_offs;
    int stream_len;
    int bits_requested;
    uint8_t encoding;
    uint8_t bfinal;
    uint8_t buf;
    uint8_t mask;
    uint8_t prev_mask;
    enum dc_state cur_state;
    enum dc_state next_state;
    enum dc_state prev_state;
    struct huff_tree tree;
};

struct dc_stream dc_init(uint8_t *, int);
void dc_step(struct dc_stream *);
void dc_get_bfinal(struct dc_stream *);
void dc_get_encoding(struct dc_stream *);

#endif
