#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

// The IDT entries are called gates.
// It can contain Interrupt Gates, Task Gates and Trap Gates.
struct idt_gate {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_2;
}__attribute__((packed));

typedef struct idt_gate idt_gate_t;

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

typedef struct idt_ptr idt_ptr_t;

void idt_bootstrap();
void idt_create_gate(int num, uint32_t offset, uint16_t selector, uint8_t type_attr);
#endif