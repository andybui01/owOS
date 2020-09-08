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

/* This defines what the stack looks like after an ISR was running */
struct regs {
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push 0xYYYY' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

void idt_bootstrap();
void idt_create_gate(int num, void *offset, uint16_t selector, uint8_t type_attr);
void fault_handler(struct regs *r);
#endif