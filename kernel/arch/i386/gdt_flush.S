#include <kernel/gdt.h>

.section .text
.align 4

// https://stackoverflow.com/questions/23978486/far-jump-in-gdt-in-bootloader

.global gdt_flush
.type gdt_flush, @function
gdt_flush:

    mov 4(%esp), %eax
    lgdt (%eax)

    mov $(SEGMENT_SELECTOR_INDEX(GDT_INDEX_DS0) | TABLE_GDT_MASK | RPL_KERNEL), %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    // Jump to flush2, an offset in the NEW code segment we just
    // put in the GDT. We do this because a MOV instruction
    // with the CS register will not suffice.
    ljmp $(SEGMENT_SELECTOR_INDEX(GDT_INDEX_CS0) | TABLE_GDT_MASK | RPL_KERNEL), $flush2

flush2:
    ret
