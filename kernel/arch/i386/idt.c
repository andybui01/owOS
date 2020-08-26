#include <kernel/idt.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// interrupt descriptor table
idt_gate_t idt[256];
idt_ptr_t ip;

extern void idt_flush(uintptr_t);

void idt_bootstrap() {
    ip.base = (uintptr_t)&idt;
    ip.limit = (sizeof(idt_gate_t) * 256) - 1;
    
    // Create empty table
    memset(&idt, 0, sizeof(idt));

    // Create keyboard interrupt handler

    idt_flush((uintptr_t)&ip);
    printf("IDT created!\n");
}

void idt_create_gate(int num, uint32_t offset, uint16_t selector, uint8_t type_attr) {

    idt[num].offset_1 = (offset & 0xFFFF);
    idt[num].offset_2 = (offset >> 16) && 0xFFFF;

    idt[num].selector = selector;

    idt[num].zero = 0;

    idt[num].type_attr = type_attr;

}