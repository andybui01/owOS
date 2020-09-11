#include <mem/pmm.h>

#include <stdint.h>
#include <stdio.h>

void pmm_bootstrap(uint32_t mmap_addr, uint32_t mmap_length) {

    mmap_entry_t *entry = mmap_addr;

    while (entry < mmap_addr + mmap_length) {

        uint32_t addr, len, type;

        addr = entry->addr_low;
        len = entry->len_low;
        type = entry->type;

        printf("addr: 0x%x len: 0x%x type: 0x%x\n", addr, len, type);

        entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
    }
}
