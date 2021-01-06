#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#ifndef __ASSEMBLER__
#include <stdint.h>

#define GDT_ENTRIES 5

// We define these variables in this order so that for the entire GDT entry
// all the bits are next to each other in memory
// i.e. this struct is effectively a bit string
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;

    uint8_t access;

	uint8_t limit_high :4;
	uint8_t flags :4;

    uint8_t base_high;
} __attribute__((packed));

typedef struct gdt_entry gdt_entry_t;

// For full definition of bit masks, goto
// https://wiki.osdev.org/Global_Descriptor_Table
#define GDT_FLAGS_SIZE 0x4
#define GDT_FLAGS_GRAN 0x8

#define GDT_ACCESS_ACCESS   0x01
#define GDT_ACCESS_RW       0x02
#define GDT_ACCESS_DC       0x04
#define GDT_ACCESS_EXEC     0x08
#define GDT_ACCESS_TYPE     0x10
#define GDT_ACCESS_DPL(x)   (x << 5)
#define GDT_ACCESS_PRESENT  0x80

// Special pointer which includes the limit:
// max bytes taken up by GDT minus 1
// THIS IS WHERE OUR GDT IS IN MEMORY!
struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

typedef struct gdt_ptr gdt_ptr_t;

extern void gdt_flush(uintptr_t);

void gdt_bootstrap();
void gdt_create_desc(unsigned int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

#endif

// 0 = kernel privilege, 3 = user privilege
#define GDT_INDEX_NULL  0
#define GDT_INDEX_CS0   1
#define GDT_INDEX_DS0   2
#define GDT_INDEX_CS3   3
#define GDT_INDEX_DS3   4
#define GDT_INDEX_TSS   5

#define TABLE_GDT_MASK 0x0
#define TABLE_LDT_MASK 0x4

#define RPL_KERNEL  0x0
#define RPL_USER    0x3

#define SEGMENT_SELECTOR_INDEX(x) (x << 3)

#endif
