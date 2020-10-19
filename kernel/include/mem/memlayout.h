#ifndef _MEM_MEMLAYOUT_H
#define _MEM_MEMLAYOUT_H

#define KMEM_BASE 0xC0000000U
#define KMEM_GRUB 0x00100000U

#define KMEM_LINK (KMEM_BASE + KMEM_GRUB)

#define V2P(x) (((uintptr_t) (x) - KMEM_BASE))
#define P2V(x) ((void *)((uintptr_t) (x) + KMEM_BASE))

#define V2P_WO(x) ((x) - KMEM_BASE)
#define P2V_WO(x) ((x) + KMEM_BASE)

#endif
