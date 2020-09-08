.section .text
.align 4

.global _gdt_flush
.type _gdt_flush, @function
_gdt_flush:

    mov 4(%esp), %eax
    lgdt (%eax)

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08,$_flush2

_flush2:
    ret
