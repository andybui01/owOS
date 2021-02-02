#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <drivers/pic.h>
#include <drivers/kbd.h>
#include <boot/multiboot.h>
#include <mem/mem.h>
#include <system/syscall.h>
#include <drivers/drivers.h>
#include <drivers/framebuffer.h>

#include <kernel/system.h>

#include <stdio.h>
#include <stdint.h>
#include <debug.h>

extern void userspace_jump(void);

void kernel_main(multiboot_info_t *mbt)
{

    // initialize terminal
    // terminal_initialize();

    fb_bootstrap(mbt);

    for (;;) {
        asm("hlt");
    }

    // get rid of blinking cursor
    // outb(0x3D4, 0x0A);
	// outb(0x3D5, 0x20);

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
    userspace_jump();

    // infinite loop ya! (privileged instruction!)
    // for (;;) {
    //     asm("hlt");
    // }

}
