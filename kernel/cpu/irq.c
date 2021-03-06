#include <kernel/irq.h>
#include <drivers/kbd.h>
#include <kernel/idt.h>
#include <int/regs.h>
#include <int/int.h>

#include <stdio.h>

static void temp_handler(regs_t*);

void irq_install_handlers(void)
{
    // Install temporary handlers
    isr_install_handler(13, &temp_handler);

    for (uint8_t i = 0; i <= INT_SIMD; ++i) {
        isr_install_handler(i, &temp_handler);
    }
}

static void temp_handler(regs_t *r)
{
    printf("Unhandled interrupt #%d err 0x%x\n", r->int_no, r->err_code);
}
