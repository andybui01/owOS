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
void idt_create_gate(int num, uint32_t offset, uint16_t selector, uint8_t type_attr);
void fault_handler(struct regs *r);
void isrs_install();

extern void _isr0();
extern void _isr1();
// extern void _isr2();
// extern void _isr3();
// extern void _isr4();
// extern void _isr5();
// extern void _isr6();
// extern void _isr7();
// extern void _isr8();
// extern void _isr9();
// extern void _isr10();
// extern void _isr11();
// extern void _isr12();
// extern void _isr13();
// extern void _isr14();
// extern void _isr15();
// extern void _isr16();
// extern void _isr17();
// extern void _isr18();
// extern void _isr19();
// extern void _isr20();
// extern void _isr21();
// extern void _isr22();
// extern void _isr23();
// extern void _isr24();
// extern void _isr25();
// extern void _isr26();
// extern void _isr27();
// extern void _isr28();
// extern void _isr29();
// extern void _isr30();
// extern void _isr31();

#endif