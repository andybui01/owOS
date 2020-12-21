#include <mem/paging.h>
#include <mem/pagetable.h>
#include <mem/memlayout.h>
#include <mem/pmm.h>
#include <stdio.h>
#include <types.h>
#include <debug.h>
#include <stdbool.h>

pgd_t *curr_dir;


// Bootstrap paging mechanism
void paging_bootstrap()
{

    // load cr3 address into C, cr3 register holds page directory address
    paddr_t cr3;
    asm volatile("mov %%cr3, %0" : "=r"(cr3));

    // set current page directory to the one we established in boot.S
    curr_dir = P2V(cr3);

    // print 768th entry, which has our higher half kernel right now
    // printf("entry 768: 0x%x\n", curr_dir->entries[768]);


    paddr_t table_addr = curr_dir->entries[768].addr << 12;

    // printf("table at 0x%x\n", table_addr);

    pgt_t *table = P2V(table_addr);

    printf("0x%x\n", table->entries[0]);
    printf("first: 0x%x last: 0x%x\n", table->entries[0], table->entries[1023]);


    return;
}

// alloc a physical frame to a virtual page
bool page_alloc(pte_t *pte)
{
    // allocate a free physical frame
    paddr_t p = pmm_frame_alloc();

    if (p == -1)
        return false;

    // map page to frame
    pte->addr = p >> 12;
    pte->present = 1;

    return true;
}

// free frame from page and invalidate page
void page_free(pte_t *pte)
{
    paddr_t p = pte->addr << 12;
    pmm_frame_dealloc(p);
    pte->present = 0;
}

// map page-aligned virtual address to  physical address
void map_addr(vaddr_t vaddr)
{
    // attempt to retrieve page table
    pde_t pde = curr_dir->entries[PAGE_DIR_INDEX(vaddr)];

    // if page table not present
    if (!pde.present) {

        // allocate frame for new page table
        paddr_t p = pmm_frame_alloc();

    }


    return;
}
