#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 5;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
bool terminal_overwrite;
 
void terminal_input_line() {
	const char* data = ">_";
	size_t size = strlen(data);
	for(size_t i = 0; i < size; i++) {
		const size_t index = (terminal_row) * VGA_WIDTH + (i + terminal_column);
		terminal_buffer[index] = vga_entry(data[i], terminal_color);
	}
}

void terminal_initialize(void) {
	terminal_overwrite = false;
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	terminal_input_line();
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_clearline(size_t y) {
	for(size_t i = 0; i < VGA_WIDTH; i++) {
		const size_t index = y * VGA_WIDTH + i;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
	}
}

void terminal_scroll(size_t row) {

	// If next row is the limit
	if (++row >= VGA_HEIGHT - 1) {

		// Shift all lines up if overwriting
		if (terminal_overwrite) {
			terminal_row = VGA_HEIGHT - 2;

			for (size_t y = 0; y < VGA_HEIGHT-2; y++) {
				for (size_t x = 0; x < VGA_WIDTH; x++) {
					const size_t index = y * VGA_WIDTH + x;
					const size_t next = index + VGA_WIDTH;
					terminal_buffer[index] = terminal_buffer[next];
				}
			}
		} else {
			terminal_overwrite = true;
			terminal_row = row;
		}
	} else {
		terminal_row = row;
	}

	// Clear next line for input
	terminal_clearline(terminal_row);
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	// Check if newline character
	if (c == '\n') {
		terminal_column = -1;
		terminal_scroll(terminal_row);
		return;
	}

	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_putchar(char c) {
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		terminal_scroll(terminal_row);
	}
}
 
void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
	terminal_input_line();
}

void kernel_main(void) {
	/* Initialize terminal interface */
	terminal_initialize();

	/* Write string to terminal */
	terminal_writestring("Zero\n");
	terminal_writestring("One\n");
	terminal_writestring("Two\n");
	terminal_writestring("Three\n");
}