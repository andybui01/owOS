#include <mem/mem.h>
#include <mem/pmm.h>
#include <boot/multiboot.h>

void mem_bootstrap(multiboot_info_t *mbt)
{
    printf("addr: 0x%x len: 0x%x\n", mbt->mmap_addr, mbt->mmap_length);
    pmm_bootstrap(mbt->mmap_addr, mbt->mmap_length);
}
