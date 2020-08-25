#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_main(void) {

	// initialize terminal
	terminal_initialize();

	// initialize GDT
	gdt_bootstrap();

}
