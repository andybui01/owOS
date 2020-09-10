#include <debug.h>
#include <stdint.h>
#include <stdio.h>

void phex(uint64_t number) {

    int nibble;
    int start = 0;

    char hex[] = "0123456789ABCDEF";

    printf("0x");

    for (int i = 0; i < 16; i++) {
        nibble = (number & 0xF000000000000000) >> 60;
        number = number << 4;
        
        if (nibble == 0 && start == 0) {
            continue;
        } else {
            start = 1;
        }

        printf("%c", hex[nibble]);
    }
    printf("\n");
}
