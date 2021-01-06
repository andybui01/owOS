#include <kernel/gdt.h>
#include <kernel/tss.h>

#include <stdint.h>
#include <stdio.h>

// global descriptor table
gdt_entry_t gdt[GDT_ENTRIES];
gdt_ptr_t gp;

void gdt_bootstrap()
{

    // Set up GDT pointer and limit
    gp.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
    gp.base = (uintptr_t) &gdt;

    // NULL descriptor
    gdt_create_desc(GDT_INDEX_NULL, 0, 0, 0, 0);

    // CODE segment descriptor
    gdt_create_desc(GDT_INDEX_CS0, 0, 0xFFFFFFFF,   (
                                                    GDT_ACCESS_PRESENT |
                                                    GDT_ACCESS_DPL(0) |
                                                    GDT_ACCESS_TYPE |
                                                    GDT_ACCESS_EXEC |
                                                    GDT_ACCESS_RW
                                                    )
                                                ,   (
                                                    GDT_FLAGS_GRAN |
                                                    GDT_FLAGS_SIZE
                                                    )
                                                );

    // DATA segment descriptor
    gdt_create_desc(GDT_INDEX_DS0, 0, 0xFFFFFFFF,   (
                                                    GDT_ACCESS_PRESENT |
                                                    GDT_ACCESS_DPL(0) |
                                                    GDT_ACCESS_TYPE |
                                                    GDT_ACCESS_RW
                                                    )
                                                ,   (
                                                    GDT_FLAGS_GRAN |
                                                    GDT_FLAGS_SIZE
                                                    )
                                                );
    // USER CODE
    gdt_create_desc(GDT_INDEX_CS3, 0, 0xFFFFFFFF,   (
                                                    GDT_ACCESS_PRESENT |
                                                    GDT_ACCESS_DPL(3) |
                                                    GDT_ACCESS_TYPE |
                                                    GDT_ACCESS_EXEC |
                                                    GDT_ACCESS_RW
                                                    )
                                                ,   (
                                                    GDT_FLAGS_GRAN |
                                                    GDT_FLAGS_SIZE
                                                    )
                                                );

    // USER DATA
    gdt_create_desc(GDT_INDEX_DS3, 0, 0xFFFFFFFF,   (
                                                    GDT_ACCESS_PRESENT |
                                                    GDT_ACCESS_DPL(3) |
                                                    GDT_ACCESS_TYPE |
                                                    GDT_ACCESS_RW
                                                    )
                                                ,   (
                                                    GDT_FLAGS_GRAN |
                                                    GDT_FLAGS_SIZE
                                                    )
                                                );

    // tss_bootstrap();

    // Flush old GDT (GRUB) and implement new one
    gdt_flush((uintptr_t) &gp);

}

// Create a GDT descriptor
void gdt_create_desc(unsigned int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    // Set the descriptor base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].limit_high = ((limit >> 16) & 0xF);

    // Granularity and size bit ALWAYS set unless NULL descriptor
    gdt[num].flags = (flags & 0xF);
    gdt[num].access = access;
}

static void tss_bootstrap(uint32_t num, uint16_t ss0, uint32_t esp0)
{
    (void) num;
    (void) ss0;
    (void) esp0;
    return;
}
