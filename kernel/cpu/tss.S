#include <kernel/gdt.h>

.section .text
.align 4

.global tss_flush
.type tss_flush, @function
tss_flush:

    mov $(SEG_SELECTOR(GDT_INDEX_TSS) | TABLE_GDT_MASK | RPL_USER), %ax
    ltr %ax
    ret
