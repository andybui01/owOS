#include <mem/memlayout.h>
#include <mem/paging.h>

// Declare constants for the multiboot header.
.set ALIGN,    1<<0             // align loaded modules on page boundaries
.set MEMINFO,  1<<1             // provide memory map
.set FLAGS,    ALIGN | MEMINFO  // this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       // 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) // checksum of above, to prove we are multiboot

// Declare a multiboot header that marks the program as a kernel.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// Allocate the initial stack.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 // 16 KiB
stack_top:

// Preallocate pages used for paging. Don't hard-code addresses and assume they
// are available, as the bootloader might have loaded its multiboot structures or
// modules there. This lets the bootloader know it must avoid the addresses.
.section .data, "aw"
    .align 4096
.global boot_page_directory
boot_page_directory:
    .fill 1024, 4, 0
boot_page_table1:
    .fill 1024, 4, 0
// Add more as necessary

// The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
    // Physical address of boot_page_table1.
    movl $V2P_WO(boot_page_table1), %edi
    movl $0, %esi
    // Map 1024 pages
    movl $1024, %ecx

.page_table_loop:
    // Map physical address as "present, writable". Note that this maps
    // .text and .rodata as writable. Mind security and map them as non-writable.
    movl %esi, %edx
    orl $(PTE_PRESENT | PTE_WRITABLE), %edx
    movl %edx, (%edi)

    // Size of page is 4096 bytes.
    addl $4096, %esi
    // Size of entries in boot_page_table1 is 4 bytes.
    addl $4, %edi

    // Map until the end of the kernel (OR MAP ALL 1024 PAGES!!)
    // cmpl $V2P_WO(_kernel_end), %esi
    // jge 3f

    // Loop to the next entry if we haven't finished.
    // this will continue until ecx = 0
    loop .page_table_loop


    // Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
    movl $V2P_WO(boot_page_table1), %ecx
    orl $(PDE_PRESENT | PDE_WRITABLE), %ecx

    movl %ecx, V2P_WO(boot_page_directory) + 0
    movl %ecx, V2P_WO(boot_page_directory) + 768 * 4

    // Make boot_page_directory the last page table
    movl $V2P_WO(boot_page_directory), %ecx
    orl $(PDE_PRESENT | PDE_WRITABLE), %ecx
    movl %ecx, V2P_WO(boot_page_directory) + 1023 * 4


    // Set cr3 to the address of the boot_page_directory.
    movl $V2P_WO(boot_page_directory), %ecx
    movl %ecx, %cr3

    // Enable paging and the write-protect bit.
    movl %cr0, %ecx
    orl $0x80000000, %ecx
    movl %ecx, %cr0

    // Jump to higher half with an absolute jump.
    lea 1f, %ecx
    jmp *%ecx

.section .text

1:
    // At this point, paging is fully set up and enabled.
    // Unmap the identity mapping as it is now unnecessary.
    movl $0, boot_page_directory + 0

    // Reload crc3 to force a TLB flush so the changes to take effect.
    movl %cr3, %ecx
    movl %ecx, %cr3

    // Set up the stack.
    movl $stack_top, %esp

    // Pass GRUB multiboot info as a virtual address.
    addl $KMEM_BASE, %ebx
    push %ebx

    // Enter the high-level kernel.
    call kernel_main

    // Infinite loop if the system has nothing more to do.
    cli
2:	hlt
    jmp 2b
