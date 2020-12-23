#include <mem/paging.h>
#include <mem/pagetable.h>
#include <mem/memlayout.h>
#include <mem/pmm.h>
#include <stdio.h>
#include <types.h>
#include <debug.h>
#include <stdbool.h>
#include <string.h>

pgd_t *curr_dir;

// flush tlb entry CURRENTLY FLUSHES ENTIRE TLB CHANGE TO INVLPG
static inline void tlb_flush(void)
{
    asm volatile(   "movl %cr3, %ecx;"
                    "movl %ecx, %cr3");
}

// Bootstrap paging mechanism
void paging_bootstrap()
{
    // set current page directory to the one at end of memory
    curr_dir = (pgd_t *) PAGE_DIR_RESERVE;

    // trigger page fault
    uint32_t *ptr = (uint32_t *) 0x1000;
    map_page((vaddr_t) 0x1000); // this line should make 0x1000 usable mem
    uint32_t trigger = *ptr;
    printf("Paging enabled...\n");
}

// alloc a physical frame to a virtual page
bool page_alloc(pte_t *pte)
{
    // allocate a free physical frame
    paddr_t p = pmm_frame_alloc();

    if (p == -1)
        return false;

    // map page to frame
    pte->addr = p >> 12; //?????
    pte->present = 1;

    return true;
}

// free frame from page and invalidate page
void page_free(pte_t *pte)
{
    paddr_t p = pte->addr << 12;//??????
    pmm_frame_dealloc(p);
    pte->present = 0;
}

// map page-aligned virtual address to  physical address
void map_page(vaddr_t vaddr)
{
    // get index of page table
    uint16_t i = PAGE_DIR_INDEX(vaddr);

    // address of page table
    vaddr_t pgt_addr = (vaddr_t) (PAGE_TABLES_RESERVE + i*0x1000);

    // if page table not present
    if (!curr_dir->entries[i].present) {

        // allocate frame for new page table
        paddr_t pgt_frame = pmm_frame_alloc();

        // map PDE to the physical frame we allocated
        curr_dir->entries[i].entry = (pgt_frame | PDE_PRESENT | PDE_WRITABLE);

        // The PGD acts as a page table for the very last 4MiB area
        // of the address space. Hence, mapping a PDE to a frame in the PGD
        // is akin to mapping the page of the page table ITSELF to the frame,
        // since all the page tables are in the last 4MiB area.

        tlb_flush();

        // zero out the entire table
        memset(pgt_addr, 0, sizeof(pgt_t));
    }

    // At this point we can assume there is a page table for the vaddr.

    // get table
    pgt_t *table = (pgt_t *) pgt_addr;

    // allocate frame for new page
    paddr_t frame = pmm_frame_alloc();
    table->entries[PAGE_TABLE_INDEX(vaddr)].entry = (frame | PTE_PRESENT | PTE_WRITABLE);

    tlb_flush();
}
