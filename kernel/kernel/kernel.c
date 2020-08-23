#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	
	/* Write string to terminal */
	terminal_writestring("Zero\n");
	terminal_writestring("One\n");
	terminal_writestring("Two\n");
	terminal_writestring("Three\n");
}
