#include <boot/multiboot.h>
#include <drivers/framebuffer.h>
#include <mem/memlayout.h>
#include <mem/paging.h>
#include <mem/pmm.h>
#include <types.h>

#include <stdio.h>

fb_info_t fb;

static void erect(uint32_t w, uint32_t h, uint32_t color);

void fb_bootstrap(multiboot_info_t *mbt)
{
    fb = mbt->fb;

    if (fb.type != MULTIBOOT_FRAMEBUFFER_TYPE_RGB) {
        printf("Failed to load LFB\n");
        return;
    }

    for (int x = 0; x < 100; x++) {
        put_pixel(x, 100, 0xFFFFFF);
    }
    // erect(100, 100, 0xFF0000);

    // At boot we assumed a framebuffer addr, here we can ENSURE it is correct
    // TODO
}

void put_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    uint32_t *offset = (uint32_t *) (FB_ADDR + y*fb.pitch + x*fb.bpp/8);
    *offset = color;

    // TODO: use the rgb position in fb_info_t
    // uint8_t *offset = (uint8_t *) (FB_ADDR + y*fb.pitch + x*fb.bpp/8);
    // *offset = color & 0xFF;                 // R
    // *(offset + 1) = (color >> 8) & 0xFF;    // G
    // *(offset + 2) = (color >> 16) & 0xFF;   // B
}
