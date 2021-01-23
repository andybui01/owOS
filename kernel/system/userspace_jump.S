#include <kernel/gdt.h>

.section .text
.align 4

.global userspace_jump
.type userspace_jump, @function
userspace_jump:
    // Save segment registers
    mov $(SEG_SELECTOR(GDT_INDEX_DS3) | RPL_USER), %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    // Store stack address in eax
    mov %esp, %eax

    // Push user data segment
    pushl $(SEG_SELECTOR(GDT_INDEX_DS3) | RPL_USER)

    // Push stack address
    pushl %eax

    // Push flags
    pushf
    pushl $(SEG_SELECTOR(GDT_INDEX_CS3) | RPL_USER)
    pushl $1f
    iret
1:
    // in user mode now, test syscall and infinite loop
    mov $0, %eax
    int $0x80
2:
    jmp 2b
