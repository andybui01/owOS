#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/system.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// interrupt descriptor table
idt_gate_t idt[256];
idt_ptr_t ip;

extern void idt_flush(uintptr_t);

static const char *exception_messages[34] = {
	"Division by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Timer",
	"Keyboard"
};

void idt_bootstrap() {
    ip.base = (uintptr_t)&idt;
    ip.limit = (sizeof(idt_gate_t) * 256) - 1;
    
    // Create empty table
    memset(&idt, 0, sizeof(idt));

    idt_flush((uintptr_t)&ip);
    printf("IDT created!\n");

	isrs_install();
}

void isrs_install() {
	printf("Instaling ISRs\n");
	idt_create_gate(0, (uint32_t) &_isr0, 0x08, 0x8E);
	idt_create_gate(1, (uint32_t) &_isr1, 0x08, 0x8E);
	idt_create_gate(32, (uint32_t) &_isr32, 0x08, 0x8E);
	idt_create_gate(33, (uint32_t) &_isr33, 0x08, 0x8E);
}

void idt_create_gate(int num, uint32_t offset, uint16_t selector, uint8_t type_attr) {

    idt[num].offset_1 = (offset & 0xFFFF);
    idt[num].offset_2 = (offset >> 16) & 0xFFFF;

    idt[num].selector = selector;

    idt[num].zero = 0;

    idt[num].type_attr = type_attr | 0x60;
}

void fault_handler(struct regs *r) {

	printf("%s\n", exception_messages[r->int_no]);

    if (r->int_no == 33) {
		unsigned char scan_code = inb(0x60);
		(void) scan_code;
		pic_send_eoi(1);
    }
}
