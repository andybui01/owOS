#include <mem/mem.h>
#include <mem/pmm.h>
#include <boot/multiboot.h>

void mem_bootstrap(multiboot_info_t *mbt)
{
    pmm_bootstrap(mbt->mmap_addr, mbt->mmap_length);
}
