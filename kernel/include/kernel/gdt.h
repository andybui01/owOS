#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

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

void gdt_bootstrap();
void gdt_create_desc(int num, uint32_t base, uint32_t limit, uint16_t access, uint8_t gran);

#endif