#ifndef _MEM_MEM_H
#define _MEM_MEM_H

#include <stdint.h>
#include <boot/multiboot.h>

#define MAX_PHYSICAL_MEM    0xFFFFFFFF
#define PAGE_SIZE           0x1000
#define N_FRAMES            MAX_PHYSICAL_MEM / PAGE_SIZE

#define PG_ALIGN(x)         (((uint32_t) x + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PG_ALIGN_DOWN(x)    (((uint32_t) x + PAGE_SIZE) & ~(PAGE_SIZE - 1))

void mem_bootstrap(multiboot_info_t *mbt);

#endif
