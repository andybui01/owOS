#include "gdt.h"
#include "tty.h"
#include <stdint.h>

// We define these variables in this order so that for the entire GDT entry
// all the bits are next to each other in memory
// i.e. this struct is effectively a bit string
struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity; // flags 55:52 and limit 51:48
	uint8_t base_high;
}__attribute__((packed));

typedef struct gdt_entry gdt_entry_t;

// Special pointer which includes the limit:
// max bytes taken up by GDT minus 1
// THIS IS WHERE OUR GDT IS IN MEMORY!
struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

typedef struct gdt_ptr gdt_ptr_t;

// Actual table
gdt_entry_t gdt[3];
gdt_ptr_t gp;

// Later on we'll call gdt_flush to put in new gdt
extern void _gdt_flush(uintptr_t);

void gdt_bootstrap() {
	
	// Set up GDT pointer and limit
	gp.limit = (sizeof(gdt_entry_t) * 3) - 1;
	gp.base = (uintptr_t) &gdt;

	// NULL descriptor
	gdt_create_desc(0, 0, 0, 0, 0);

	// Since we're using a flat setup, the code and data segment overlaps.
	// Effectively, both have limits of 4KB and a base of 0.

	// CODE segment descriptor
	gdt_create_desc(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	// DATA segment descriptor
	gdt_create_desc(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	// TODO: add non/privileged code/data segments? (all overlapping)

	// Flush old GDT (GRUB) and implement new one
	_gdt_flush((uintptr_t) &gp);

	terminal_writestring("GDT created!\n");

}

// Create a GDT descriptor
void gdt_create_desc(int num, uint32_t base, uint32_t limit, uint16_t access, uint8_t gran) {

	// Set the descriptor base address
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	/* Setup the descriptor limits */
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	/* Finally, set up the granularity and access flags */
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}