#ifndef _SYSTEM_GDT_H
#define _SYSTEM_GDT_H

#include <stdint.h>
#include <stdbool.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
bool are_interrupts_enabled();

#endif