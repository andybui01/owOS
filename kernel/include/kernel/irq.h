#ifndef _KERNEL_IRQ_H
#define _KERNEL_IRQ_H

#include <int/regs.h>

// Wrapper for handler function pointer
typedef void (*irq_handler_t) (regs_t *);

void irq_install_handlers(void);

#endif
