#include <kernel/idt.h>
#include <kernel/irq.h>
#include <int/regs.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// interrupt descriptor table
idt_gate_t idt[256];
idt_ptr_t ip;

// table for C-level fault handlers
irq_handler_t handlers[256] = {0};

void idt_bootstrap() 
{
    ip.base = (uintptr_t)&idt;
    ip.limit = (sizeof(idt_gate_t) * 256) - 1;
    
    // Create empty table
    memset(&idt, 0, sizeof(idt));

    idt_flush((uintptr_t)&ip);

	isrs_install();
}

void isrs_install() 
{
	idt_create_gate(0, (uint32_t) &_isr0, 0x08, 0x8E);
	idt_create_gate(1, (uint32_t) &_isr1, 0x08, 0x8E);
	idt_create_gate(32, (uint32_t) &_isr32, 0x08, 0x8E);
	idt_create_gate(33, (uint32_t) &_isr33, 0x08, 0x8E);

	irq_install_handlers();
}

void idt_create_gate(int num, uint32_t offset, uint16_t selector, uint8_t type_attr) 
{

    idt[num].offset_1 = (offset & 0xFFFF);
    idt[num].offset_2 = (offset >> 16) & 0xFFFF;

    idt[num].selector = selector;

    idt[num].zero = 0;

    idt[num].type_attr = type_attr | 0x60; // force to ring 0: change later
}

void isr_install_handler(int index, irq_handler_t handler) 
{
	handlers[index] = handler;
}

void isr_uninstall_handler(int index) 
{
	handlers[index] = 0;
}

void fault_handler(regs_t *r) 
{

	irq_handler_t handler = handlers[r->int_no];

	if (handler) {
		handler(r);
	} else {
		// unhandled exception, die
		printf("unhandled exception #%d: dying :(\n", r->int_no);
		for (;;);
	}
}
