#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <drivers/pic.h>
#include <drivers/kbd.h>
#include <boot/multiboot.h>
#include <mem/mem.h>

#include <stdio.h>
#include <stdint.h>
#include <debug.h>

void kernel_main(multiboot_info_t *mbt)
{

    // initialize terminal
    terminal_initialize();

    // initialize GDT
    gdt_bootstrap();

    // initialize IDT
    idt_bootstrap();

    // remap PIC
    pic_remap(0x20, 0x28);

    mem_bootstrap(mbt);

    // infinite loop ya!
    for (;;) {
        asm("hlt");
    }

}
