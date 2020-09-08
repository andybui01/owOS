#ifndef _DRIVERS_KBD_H
#define _DRIVERS_KBD_H

#include <kernel/idt.h>
#include <int/regs.h>

void keyboard_handler(regs_t *r);

#endif
