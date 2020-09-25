#ifndef _PMM_H
#define _PMM_H

#include <boot/multiboot.h>
#include <stdint.h>
#include <stdbool.h>

void pmm_bootstrap(uint32_t mmap_addr, uint32_t mmap_length);
uint32_t pmm_frame_alloc(void);
void pmm_frame_dealloc(uint32_t faddr);
bool pmm_check_frame(uint32_t faddr);

#endif
