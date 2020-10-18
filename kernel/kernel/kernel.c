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

extern uint32_t _kernel_end;

void kernel_main(multiboot_info_t *mbt)
{

    // initialize terminal
    terminal_initialize();
    printf("hi\n");


    // mem_bootstrap(mbt);

    // initialize GDT
    gdt_bootstrap();

    // initialize IDT
    idt_bootstrap();
    
    kbreak();
    // printf("0x%x\n", mbt->mmap_addr);
    printf("kernel ends at: 0x%x\n", _kernel_end);
    int a = 5/0;
    (void) a;


    // remap PIC
    pic_remap(0x20, 0x28);

    // infinite loop ya!
    for (;;) {
        asm("hlt");
    }

}
