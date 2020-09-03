#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>

#include <debug.h>

void kernel_main(void) {

	// initialize terminal
	terminal_initialize();

	// initialize GDT
	gdt_bootstrap();

	// initialize IDT
	idt_bootstrap();

	// install fault handlers
	isrs_install();

	// remap PIC
	pic_remap(0x20, 0x28);

	// try to trigger exception handler with a
	// division by zero error
	// int a = 5/0;
	// (void) a;

	// infinite loop ya!
	for (;;) {
		asm("hlt");
	}

}
