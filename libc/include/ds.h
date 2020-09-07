#ifndef _DS_H
#define _DS_H 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct circ_bbuf {
    uint8_t *const buffer;
    int head;
    int tail;
    const int maxlen;
};

typedef struct circ_bbuf circ_bbuf_t;

int circ_bbuf_push(circ_bbuf_t *c, uint8_t data);
int circ_bbuf_pop(circ_bbuf_t *c, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif