#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <int/regs.h>
#include <int/int.h>

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
    // For full list of exceptions visit: https://wiki.osdev.org/Exceptions
    idt_create_gate(INT_DIVIDE_ZERO,    (uint32_t) &_isr0, SEG_SELECTOR(GDT_INDEX_CS0), DPL_KERNEL);
    idt_create_gate(INT_GPF,            (uint32_t) &_isr13, SEG_SELECTOR(GDT_INDEX_CS0), DPL_KERNEL);
    idt_create_gate(INT_PAGE_FAULT,     (uint32_t) &_isr14, SEG_SELECTOR(GDT_INDEX_CS0), DPL_KERNEL);
    idt_create_gate(INT_KBD,            (uint32_t) &_isr33, SEG_SELECTOR(GDT_INDEX_CS0), DPL_KERNEL);
    idt_create_gate(INT_SYSCALL,        (uint32_t) &_isr128, SEG_SELECTOR(GDT_INDEX_CS0), DPL_USER);

    irq_install_handlers();
}

void idt_create_gate(int num, uint32_t offset, uint16_t selector, uint8_t dpl)
{

    idt[num].offset_1 = (offset & 0xFFFF);
    idt[num].offset_2 = (offset >> 16) & 0xFFFF;

    idt[num].selector = selector;

    idt[num].zero = 0;

    idt[num].type_attr = (IDT_PRESENT | IDT_DPL(dpl) | IDT_SIZE | IDT_INT);
}

void isr_install_handler(int index, irq_handler_t handler)
{
    if (idt[index].selector)
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
    }
}
