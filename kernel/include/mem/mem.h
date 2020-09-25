#ifndef _MEM_H
#define _MEM_H

#include <stdint.h>

#define MAX_PHYSICAL_MEM    0xFFFFFFFF
#define PAGE_SIZE           0x1000
#define N_FRAMES            MAX_PHYSICAL_MEM / PAGE_SIZE 

#define FRAME_ADDR(x)       ((x + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

#endif
