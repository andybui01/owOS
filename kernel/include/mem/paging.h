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

#ifndef __ASSEMBLER__

#include <stdbool.h>
#include <mem/pagetable.h>
#include <types.h>
#include <int/regs.h>

void paging_bootstrap();
bool page_alloc(pte_t *pte);
void page_free(pte_t *pte);
void map_page(vaddr_t vaddr);
void page_fault_handler(regs_t *r);


#define PAGE_INDEX_MASK 0x3ff

#define PAGE_DIR_INDEX(x) (((uint32_t)(x) >> 22) & PAGE_INDEX_MASK)
#define PAGE_TABLE_INDEX(x) (((uint32_t)(x) >> 12) & PAGE_INDEX_MASK)

#define PAGE_DIR_INDEX_WO(x) (((x) >> 22) & PAGE_INDEX_MASK)
#define PAGE_TABLE_INDEX_WO(x) (((x) >> 12) & PAGE_INDEX_MASK)


#define PF_ERR_PRESENT  0x1
#define PF_ERR_WRITE    0x2
#define PF_ERR_USER     0x4
#define PF_ERR_RESERVED 0x8
#define PF_ERR_FETCH    0x10

#define pf_err_not_present(x)   (x & PF_ERR_PRESENT)
#define pf_err_was_write(x)         (x & PF_ERR_WRITE)
#define pf_err_was_user(x)          (x & PF_ERR_USER)

#endif

#endif
