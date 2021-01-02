#include <drivers/kbd.h>
#include <drivers/pic.h>
#include <int/regs.h>
#include <ds.h>
#include <stdint.h>

// temp
#include <stdio.h>

// Some useful defines
#define KB_SHIFT_LEFT   0x2A
#define KB_SHIFT_RIGHT  0x36
#define KEY_RELEASE     0x80
#define KEY_ACTUAL      0x7F

// This buffer will contain the actual converted ASCII bytes from input
uint8_t inbuffer_data_space[32];
circ_bbuf_t inbuffer = {
    .buffer = inbuffer_data_space,
    .head = 0,
    .tail = 0,
    .maxlen = 32
};

// borrowing bran's scan codes table
unsigned char sc_qwerty[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
    '9', '0', '-', '=', '\b',	/* Backspace */
    '\t',			/* Tab */
    'q', 'w', 'e', 'r',	/* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
    '\'', '`',   0,		/* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
    'm', ',', '.', '/',   0,				/* Right shift */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned char sc_qwerty_shifted[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
    '(', ')', '_', '+', '\b',	/* Backspace */
    '\t',			/* Tab */
    'Q', 'W', 'E', 'R',	/* 19 */
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* ENTER KEY */
    0,			/* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
    '\"', '~',   0,		/* Left shift */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
    'M', '<', '>', '?',   0,				/* RIGHT SHIFT */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

// flags
uint8_t shift_flag = 0;

void keyboard_handler(regs_t *r)
{
    (void) r;
    uint8_t scan_code = pic_scan_code();

    // If detect key press
    if (!(scan_code & KEY_RELEASE)) {

        unsigned char byte;

        if (scan_code == KB_SHIFT_LEFT || scan_code == KB_SHIFT_RIGHT) {
            shift_flag = scan_code;
        } else {

            if (shift_flag)
                byte = sc_qwerty_shifted[scan_code];
            else
                byte = sc_qwerty[scan_code];

            // add to circular buffer for applications to deal with
            circ_bbuf_push(&inbuffer, byte);
            printf("%c", byte);
        }
    }

    // Only care about shift release for now
    if (scan_code & KEY_RELEASE) {
        scan_code &= KEY_ACTUAL;
        if (scan_code == KB_SHIFT_LEFT || scan_code == KB_SHIFT_RIGHT) {
            shift_flag = 0;
        }
    }

    pic_send_eoi(1);
}
