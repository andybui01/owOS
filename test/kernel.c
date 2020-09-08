
#include "tty.h"
#include "gdt.h"

void kernel_main(void) {

	// initialize terminal
	terminal_initialize();
	terminal_writestring("terminal initialized\n");

	gdt_bootstrap();

	// printf("lol\n");

	// initialize GDT
}
