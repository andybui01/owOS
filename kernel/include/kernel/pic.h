#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H

#include <stdint.h>

void pic_remap(uint8_t offset_m, uint8_t offset_s);
void pic_send_eoi(uint8_t irq);

#endif