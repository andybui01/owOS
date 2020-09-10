#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <drivers/pic.h>
#include <drivers/kbd.h>
#include <kernel/multiboot.h>

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

    mmap_entry_t *entry = mbt->mmap_addr;
    while (entry < mbt->mmap_addr + mbt->mmap_length) {

        uint64_t addr;
        
        addr = entry->addr_high;
        addr = addr << 32 | entry->addr_low;

        printf("addr_lo: 0x%x addr_hi: 0x%x len_lo: 0x%x len_hi: 0x%x\n",
            entry->addr_low,
            entry->addr_high,
            entry->len_low,
            entry->len_high
        );

        entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
    }

    // infinite loop ya!
    for (;;) {
        asm("hlt");
    }

}
