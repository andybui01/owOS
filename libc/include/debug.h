#ifndef _DEBUG_H
#define _DEBUG_H 1

#include <sys/cdefs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void kbreak();
void phex(uint64_t);

#ifdef __cplusplus
}
#endif

#endif
