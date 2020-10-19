#include <mem/mem.h>
#include <mem/pmm.h>
#include <boot/multiboot.h>
#include <stdio.h>
#include <mem/memlayout.h>

void mem_bootstrap(multiboot_info_t *mbt)
{
    // mbt mmap_addr is still a physical address, so we need to be careful
    printf("addr: 0x%x len: 0x%x\n", P2V_WO(mbt->mmap_addr), mbt->mmap_length);
    pmm_bootstrap(P2V(mbt->mmap_addr), mbt->mmap_length);
}
