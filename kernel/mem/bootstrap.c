#include <mem/mem.h>
#include <mem/pmm.h>
#include <boot/multiboot.h>
#include <stdio.h>
#include <mem/memlayout.h>
#include <mem/paging.h>
#include <mem/pagetable.h>

void mem_bootstrap(multiboot_info_t *mbt)
{
    // mbt mmap_addr is still a physical address, so we need to be careful
    pmm_bootstrap(P2V_WO(mbt->mmap_addr), mbt->mmap_length);

    paging_bootstrap();
}
