#ifndef _MEM_MEMLAYOUT_H
#define _MEM_MEMLAYOUT_H

#define KMEM_BASE 0xC0000000
#define KMEM_GRUB 0x00100000

#define KMEM_LINK (KMEM_BASE + KMEM_GRUB)

// Linear frame buffer reserve
#define FB_ADDR_RESERVE     0xFF800000

#define PAGE_TABLES_RESERVE 0xFFC00000
#define PAGE_DIR_RESERVE    0xFFFFF000

#define KHEAP_END           FB_ADDR_RESERVE

#define V2P(x) (((uint32_t) (x) - KMEM_BASE))
#define P2V(x) ((void *)((uint32_t) (x) + KMEM_BASE))

#define V2P_WO(x) ((x) - KMEM_BASE)
#define P2V_WO(x) ((x) + KMEM_BASE)

#endif
