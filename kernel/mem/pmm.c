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

void pmm_bootstrap(uint32_t mmap_addr, uint32_t mmap_length)
{

    mmap_entry_t *entry = mmap_addr;

    uint32_t n = 0;
    uint32_t n2 = 0;

    for (; entry < mmap_addr + mmap_length; entry++) {

        if (entry->type != MULTIBOOT_MEMORY_AVAILABLE)
            continue;

        uint32_t addr, len;
        addr = entry->len_low;
        len = entry->len_low;

        n2 += len / PAGE_SIZE;

        uint32_t paddr = ((addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1));

        for (; paddr < addr + len; paddr += PAGE_SIZE) {
            
            // Grab index and offset of bitmap
            uint32_t index = paddr / PAGE_SIZE;
            uint32_t offset = paddr % 8;
            index /= 8;

            // Set frame status as "available"
            frame_bitmap[index] &= (1 << offset);
            n++;
        }
    }

    printf("There are %d/%d allocated\n", n, n2 );
}
