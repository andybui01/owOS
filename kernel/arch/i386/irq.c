#include <kernel/irq.h>
#include <drivers/kbd.h>
#include <kernel/idt.h>
#include <int/regs.h>

#define KEYBOARD_INT 33

extern void keyboard_handler(regs_t *);

void irq_install_handlers(void) {
    irq_handler_t handler;

    handler = &keyboard_handler;
    isr_install_handler(KEYBOARD_INT, handler);
}
