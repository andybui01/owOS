#ifndef _MEM_PAGING_H
#define _MEM_PAGING_H

#define PDE_PRESENT        0x001
#define PDE_WRITABLE       0x002
#define PDE_USER           0x004
#define PDE_WRITE_THROUGH  0x008
#define PDE_CACHE_DISABLE  0x010
#define PDE_ACCESSED       0x020
#define PDE_PAGE_SIZE      0x080
#define PDE_GLOBAL         0x100
#define PDE_RESERVED       0x040

#define PTE_PRESENT        0x001
#define PTE_WRITABLE       0x002
#define PTE_USER           0x004
#define PTE_WRITE_THROUGH  0x008
#define PTE_CACHE_DISABLE  0x010
#define PTE_ACCESSED       0x020
#define PTE_DIRTY          0x040
#define PTE_PAT_BIT_3      0x080
#define PTE_GLOBAL         0x100

#endif
