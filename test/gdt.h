#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>

void gdt_bootstrap();
void gdt_create_desc(int num, uint32_t base, uint32_t limit, uint16_t access, uint8_t gran);

#endif