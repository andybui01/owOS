#include <stdlib.h>
#include <stdint.h>

void itox(uint64_t number, char* buffer) {

    int nibble;
    int index = 0;

    char hex[] = "0123456789ABCDEF";

    for (int i = 0; i < 8; i++) {
        nibble = (number & 0xF0000000) >> 28;
        number = number << 4;
        
        if (nibble == 0 && index == 0) {
            continue;
        }

        buffer[index] = hex[nibble];
        ++index;
    }
    
    if (!index) {
        buffer[index] = '0';
        ++index;
    }
    buffer[index] = '\0';
}