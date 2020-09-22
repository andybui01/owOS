#ifndef _PMM_H
#define _PMM_H

#include <kernel/multiboot.h>
#include <stdint.h>

void pmm_bootstrap(uint32_t mmap_addr, uint32_t mmap_length);

#endif
