#include <mem/pmm.h>
#include <mem/mem.h>

#include <stdint.h>
#include <stdio.h>

#define MULTIBOOT_MEMORY_AVAILABLE          1
#define MULTIBOOT_MEMORY_RESERVED           2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE   3
#define MULTIBOOT_MEMORY_NVS                4
#define MULTIBOOT_MEMORY_BADRAM             5

// uint32_t???
uint8_t frame_bitmap[N_FRAMES/8];

void pmm_bootstrap(uint32_t mmap_addr, uint32_t mmap_length) {

    mmap_entry_t *entry = mmap_addr;

    uint32_t n = 0;

    while (entry < mmap_addr + mmap_length) {

        uint32_t addr, len, type;

        addr = entry->addr_low;
        len = entry->len_low;
        type = entry->type;

        printf("addr: 0x%x len: 0x%x type: 0x%x\n", addr, len, type);

        if (type == MULTIBOOT_MEMORY_AVAILABLE) {
            uint32_t ptr = addr + (0x1000 - (addr % 0x1000)) + 0x1000;
            // for (; ptr < addr + len; ptr += 0x1000) {
            // 	++n;
            // }
            n += len / 0x1000;
        }

        entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
    }

    printf("There are %d/%d frames\n", n, N_FRAMES);
}
