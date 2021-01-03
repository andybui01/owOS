#include <kernel/gdt.h>
#include <stdint.h>
#include <stdio.h>

// global descriptor table
gdt_entry_t gdt[3];
gdt_ptr_t gp;

void gdt_bootstrap()
{

    // Set up GDT pointer and limit
    gp.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gp.base = (uintptr_t) &gdt;

    // NULL descriptor
    gdt_create_desc(0, 0, 0, 0, 0);

    // CODE segment descriptor
    gdt_create_desc(1, 0, 0xFFFFFFFF, 0x9A, 0xC);

    // DATA segment descriptor
    gdt_create_desc(2, 0, 0xFFFFFFFF, 0x92, 0xC);

    // USER CODE
    gdt_create_desc(3, 0, 0xFFFFFFFF, 0xFA, 0xC);

    // USER DATA
    gdt_create_desc(4, 0, 0xFFFFFFFF, 0xF2, 0xC);

    tss_bootstrap();

    // Flush old GDT (GRUB) and implement new one
    gdt_flush((uintptr_t) &gp);

}

// Create a GDT descriptor
void gdt_create_desc(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    // Set the descriptor base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].limit_high = ((limit >> 16) & 0xF);

    /* Finally, set up the granularity and access flags */
    gdt[num].flags = (flags & 0xF);
    gdt[num].access = access;
}
