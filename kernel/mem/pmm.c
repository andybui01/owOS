#include <mem/pmm.h>
#include <mem/mem.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <debug.h>

#define MULTIBOOT_MEMORY_AVAILABLE          1
#define MULTIBOOT_MEMORY_RESERVED           2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE   3
#define MULTIBOOT_MEMORY_NVS                4
#define MULTIBOOT_MEMORY_BADRAM             5

uint8_t frame_bitmap[N_FRAMES/8] = {0};
uint32_t last_index = 0;

void pmm_bootstrap(uint32_t mmap_addr, uint32_t mmap_length)
{
    // memset(frame_bitmap, 0, sizeof(frame_bitmap));

    mmap_entry_t *entry = mmap_addr;

    for (; entry < mmap_addr + mmap_length; entry++) {

        if (entry->type != MULTIBOOT_MEMORY_AVAILABLE)
            continue;

        uint32_t addr, len;
        addr = entry->len_low;
        len = entry->len_low;

        printf("addr: 0x%x\n", addr);

        uint32_t faddr = FRAME_ADDR(addr);
        printf("first faddr: 0x%x\n", faddr);

        for (; faddr < addr + len; faddr += PAGE_SIZE) {

            // Grab index and offset of bitmap
            uint32_t index = faddr / PAGE_SIZE;
            uint32_t off = index % 8;
            index /= 8;

            kbreak();

            // Set frame status as "available"
            frame_bitmap[index] |= (1 << off);
        }
    }
}

uint32_t pmm_frame_alloc(void)
{
    for (uint32_t index = last_index; index < N_FRAMES / 8; ++index) {

        if (frame_bitmap[index]) {

            // Walk along bitstring and find available frame
            for (uint8_t off = 0; off < 8; off++) {

                if (frame_bitmap[index] & (1 << off)) {
                    printf("FRAME before: 0x%x\n", frame_bitmap[index]);

                    // Flip the bit
                    frame_bitmap[index] ^= (1 << off);
                    printf("FRAME after: 0x%x\n", frame_bitmap[index]);

                    // Backup index and return frame address
                    last_index = index;
                    return (index*8 + off) * PAGE_SIZE;
                }
            }
        }
    }

    return 0;
}

void pmm_frame_dealloc(uint32_t faddr)
{
    uint32_t index = (faddr / PAGE_SIZE) / 8;
    uint8_t off = index % 8;

    // flip bit
    frame_bitmap[index] ^= (1 << off);
}

bool pmm_check_frame(uint32_t faddr)
{

    uint32_t index = (faddr / PAGE_SIZE) / 8;
    uint8_t off = index % 8;

    return (frame_bitmap[index] & (1 << off));
}
