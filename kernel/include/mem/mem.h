#ifndef _MEM_H
#define _MEM_H

#include <stdint.h>

#define MAX_PHYSICAL_MEM    0xFFFFFFFF
#define PAGE_SIZE           0x1000
#define N_FRAMES            MAX_PHYSICAL_MEM / PAGE_SIZE 

#endif
