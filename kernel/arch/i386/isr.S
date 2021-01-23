.section .text
.align 4

// .global isr1
// .type isr1, @function

// isr1:
//     cli // Disable interrupts until we finish handling this one
//     push $0x0000 // Error code
//     push $0x0001 // INT number
//     jmp isr_common

.macro ISR_NOERR index
    .global _isr\index
    _isr\index:
        cli
        push $0
        push $\index
        jmp isr_common
.endm

.macro ISR_ERR index
    .global _isr\index
    _isr\index:
        cli
        push $\index
        jmp isr_common
.endm

/* Standard X86 interrupt service routines */
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31
ISR_NOERR 32
ISR_NOERR 33

ISR_NOERR 128

.extern fault_handler
.type fault_handler, @function

isr_common:

    //////////////////////////
    // SAVE PROCESSOR STATE //
    //////////////////////////

    // Push all general purpose registers onto the stack
    // in the following order: eax,ecx,edx,ebx,esp,esi,edi
    pusha

    push %ds
    push %es
    push %fs
    push %gs

    /////////////////////////////////////
    // SET UP FOR KERNEL MODE SEGMENTS //
    /////////////////////////////////////

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    // mov %esp, %eax
    // push %eax
    cld

    //////////////////////////
    // Call C fault handler //
    //////////////////////////

    // mov $fault_handler, %eax
    // call *%eax
    push %esp
    call fault_handler
    add $4, %esp

    /////////////////////////
    // RESTORE STACK FRAME //
    /////////////////////////

    // pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa // Pop all general purpose registers

    add $8, %esp // Clean up pushed error code and INT number
    iret
