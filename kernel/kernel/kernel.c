#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <drivers/pic.h>
#include <drivers/kbd.h>
#include <boot/multiboot.h>
#include <mem/mem.h>
#include <system/syscall.h>
#include <drivers/drivers.h>

#include <kernel/system.h>

#include <stdio.h>
#include <stdint.h>
#include <debug.h>

static void switch_to_user_mode(void);

void kernel_main(multiboot_info_t *mbt)
{

    // initialize terminal
    terminal_initialize();

    // get rid of blinking cursor
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);

    // initialize GDT
    gdt_bootstrap();

    // initialize IDT
    idt_bootstrap();

    // remap PIC
    pic_remap(0x20, 0x28);

    // initialize drivers
    drivers_bootstrap();

    mem_bootstrap(mbt);

    // initialize syscall interface
    syscall_bootstrap();

    // jump to userspace
    switch_to_user_mode();

    // syscall test
    asm volatile(" \
        xor %eax, %eax; \
        int $0x80; \
        ");

    // infinite loop ya!
    for (;;) {
        asm("hlt");
    }

}

static void switch_to_user_mode(void)
{
    // Set up a stack structure for switching to user mode.
    asm volatile("  \
        // cli; \
        mov $0x23, %ax; \
        mov %ax, %ds; \
        mov %ax, %es; \
        mov %ax, %fs; \
        mov %ax, %gs; \
                    \
        mov %esp, %eax; \
        pushl $0x23; \
        pushl %eax; \
        pushf; \
        // pop %eax; \
        // or %eax, $0x200; \
        // push %eax; \
        pushl $0x1B; \
        push $1f; \
        iret; \
    1: \
    ");
}
