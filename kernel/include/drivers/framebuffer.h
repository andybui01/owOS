#ifndef _DRIVERS_FRAMEBUFFER_H
#define _DRIVERS_FRAMEBUFFER_H

#include <boot/multiboot.h>
#include <mem/memlayout.h>

#define FB_ADDR FB_ADDR_RESERVE

void fb_bootstrap(multiboot_info_t *mbt);
void put_pixel(uint32_t x, uint32_t y, uint32_t color);

#endif
