#ifndef _MEM_HEAP_H
#define _MEM_HEAP_H

#include <types.h>

// simple upwards bump allocator implementation
struct bump_allocator {
    vaddr_t curr;
    vaddr_t end;
    vaddr_t last_page;
};

typedef struct bump_allocator heap_t;

void heap_bootstrap(void);
void *kmalloc(uint32_t size);
void kfree(void *ptr);

#endif
