#include <mem/pmm.h>
#include <mem/mem.h>
#include <mem/memlayout.h>

#include <types.h>
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


// kernel start and end addresses
extern uint32_t _kernel_start, _kernel_end;
static vaddr_t kstart_addr = &_kernel_start;
static vaddr_t kend_addr = &_kernel_end;

uint8_t frame_bitmap[N_FRAMES/8] = {0};
uint32_t last_index = 0;

void pmm_bootstrap(uint32_t mmap_addr, uint32_t mmap_length)
{
    paddr_t kstart_pg = FRAME_ALIGN_DOWN(V2P(kstart_addr));
    paddr_t kend_pg = FRAME_ALIGN(V2P(kend_addr));

    mmap_entry_t *entry = (mmap_entry_t *) mmap_addr;

    // skip first entry because its the GRUB reserved area
    entry++;

    for (; (uint32_t) entry < mmap_addr + mmap_length; entry++) {

        if (entry->type != MULTIBOOT_MEMORY_AVAILABLE)
            continue;

        uint32_t addr, len;
        addr = entry->addr_low;
        len = entry->len_low;

        // if region exists in kernel's locaton, skip
        if (addr >= kstart_pg && (addr + len) < kend_pg)
            continue;

        // printf("kstart: 0x%x kend: 0x%x\n", kstart_pg, kend_pg);
        // printf("addr: 0x%x len: 0x%x type: 0x%x\n", entry->addr_low, entry->len_low, entry->type);

        uint32_t faddr = FRAME_ALIGN(addr);

        for (; faddr < addr + len; faddr += PAGE_SIZE) {

            // skip if we're starting on 1MiB mark
            if (faddr == 0x100000)
                continue;

            // if region is at kstart, skip to kend
            if (faddr == kstart_pg) {
                faddr = kend_pg - PAGE_SIZE;
                continue;
            }

            // Grab index and offset of bitmap
            uint32_t index = faddr / PAGE_SIZE;
            uint32_t off = index % 8;
            index /= 8;

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

                    // Set bit to 0
                    frame_bitmap[index] ^= (1 << off);

                    // Backup index and return frame address
                    last_index = index;
                    return (index*8 + off) * PAGE_SIZE;
                }
            }
        }
    }

    // since this function returns page aligned addresses,
    // we can use this to signal an error
    return 1;
}

void pmm_frame_dealloc(uint32_t faddr)
{
    uint32_t index = (faddr / PAGE_SIZE) / 8;
    uint8_t off = index % 8;

    // set bit to 1
    frame_bitmap[index] |= (1 << off);
}

bool pmm_check_frame(uint32_t faddr)
{

    uint32_t index = (faddr / PAGE_SIZE) / 8;
    uint8_t off = index % 8;

    return (frame_bitmap[index] & (1 << off));
}
