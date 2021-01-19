#include <kernel/gdt.h>
#include <kernel/tss.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// assembly functions
extern void gdt_flush(uintptr_t);
extern void tss_flush(void);

// static function declarations
static void tss_write(unsigned int, uint16_t, uint32_t);

// global descriptor table
gdt_entry_t gdt[GDT_ENTRIES];
gdt_ptr_t gp;

tss_entry_t tss;

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

    tss_write(GDT_INDEX_TSS, 0x10, 0x0);

    // Flush old GDT (GRUB) and implement new one
    gdt_flush((uintptr_t) &gp);
    tss_flush();

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

// this does nothing for now except create a descriptor in the GDT
static void tss_write(unsigned int num, uint16_t ss0, uint32_t esp0)
{
    uint32_t base = (uint32_t) &tss;
    uint32_t limit = base + sizeof(tss_entry_t);

    gdt_create_desc(num, base, limit,   (
                                            GDT_ACCESS_PRESENT |
                                            GDT_ACCESS_DPL(3) |
                                            GDT_ACCESS_EXEC |
                                            GDT_ACCESS_ACCESS
                                        ),
                                        0
                                    );
    memset(&tss, 0, sizeof(tss_entry_t));
    tss.ss0 = ss0;
    tss.esp0 = esp0;
    tss.cs = 0x0b;

	tss.ss =
	tss.ds =
	tss.es =
	tss.fs =
	tss.gs = 0x13;
}
