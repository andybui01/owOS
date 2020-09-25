#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <drivers/pic.h>
#include <drivers/kbd.h>
#include <kernel/multiboot.h>
#include <mem/pmm.h>

#include <stdio.h>
#include <stdint.h>
#include <debug.h>

void kernel_main(multiboot_info_t *mbt) {

    // initialize terminal
    terminal_initialize();

    // initialize GDT
    gdt_bootstrap();

    // initialize IDT
    idt_bootstrap();

    // remap PIC
    pic_remap(0x20, 0x28);

    pmm_bootstrap(mbt->mmap_addr, mbt->mmap_length);

    uint32_t faddr = pmm_frame_alloc();

    printf("allocated 0x%x\n", faddr);

    if (!pmm_check_frame(faddr)) {
        printf("frame in use at 0x%x\n", faddr);
    }

    pmm_frame_dealloc(faddr);

    if (pmm_check_frame(faddr)) {
        printf("frame NOT in use at 0x%x\n", faddr);
    }


    // infinite loop ya!
    for (;;) {
        asm("hlt");
    }

}
