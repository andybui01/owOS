#include <drivers/kbd.h>
#include <kernel/pic.h>
#include <int/regs.h>

#include <stdint.h>

void keyboard_handler(regs_t *r) {
    (void) r;
    uint8_t scan_code = pic_scan_code();
    (void) scan_code;
    pic_send_eoi(1);
}