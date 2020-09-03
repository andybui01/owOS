#ifndef _KERNEL_SYSTEM_H
#define _KERNEL_SYSTEM_H

#include <stdint.h>
#include <stdbool.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait(void);
bool are_interrupts_enabled(void);

#endif