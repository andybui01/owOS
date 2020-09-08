#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H

#include <stdint.h>

void pic_remap(uint8_t offset_m, uint8_t offset_s);
void pic_send_eoi(uint8_t irq);
void irq_set_mask(uint8_t irq);
void irq_clear_mask(uint8_t irq);
uint8_t pic_scan_code();

#endif
