#include <drivers/pic.h>
#include <kernel/system.h>

#define MPIC 0x0020
#define SPIC 0x00A0

#define MPIC_COMMAND MPIC
#define MPIC_DATA (MPIC+1)  // Set A0 bit to 1

#define SPIC_COMMAND SPIC
#define SPIC_DATA (SPIC+1)  // Set A0 bit to 1

#define PIC_EOI     0x20

#define ICW1_IC4    0x01    // ICW4 bit
#define ICW1_SNGL   0x02    // Single bit, if not then cascade mode
#define ICW1_ADI    0x04    // Address interval 1 if set or 8 if not
#define ICW1_LTIM   0x08    // Level interript or edge detect logic
#define ICW1_INIT   0x10    // Signify to PIC to reinitialise

// Full definitions can be found on page 11 in the document linked below
#define ICW4_UPM    0x01    // microprocessor mode, 8086/88 if set, otherwise MCS-80/85
#define ICW4_AEOI   0x02    // auto EOI, if not normal EOI
#define ICW4_MS     0x04    // If buffered mode, 8259A is master, else slave.
#define ICW4_BUFR   0x08    // Enable buffered mode
#define ICW4_SFNM   0x10    // Special fully nested mode

/*
    Read: https://pdos.csail.mit.edu/6.828/2014/readings/hardware/8259A.pdf
    pic_remap() follows the flow chart in Figure 6.

    uint8_t offset_m:   new offset for master PIC, vectors on the master PIC
                        will become offset_m .. offset_m + 7
    uint8_t offset_s:   new offset for slave PIC, vectors on the slave PIC
                        will become offset_s .. offset_s + 7
*/
void pic_remap(uint8_t offset_m, uint8_t offset_s) {
    
    // backup masks
    uint8_t backup_m = inb(MPIC_DATA);
    uint8_t backup_s = inb(SPIC_DATA);

    // ICW1, initialise in cascade mode
    outb(MPIC_COMMAND, ICW1_INIT | ICW1_IC4);
    io_wait();
    outb(SPIC_COMMAND, ICW1_INIT | ICW1_IC4);
    io_wait();

    // ICW2, set new offset
    outb(MPIC_DATA, offset_m);
    io_wait();
    outb(SPIC_DATA, offset_s);
    io_wait();

    // ICW3
    outb(MPIC_DATA, 0b00000100); // MPIC has a single slave at IRQ2
    io_wait();
    outb(SPIC_DATA, 0b00000010); // SPIC has a casecade identity of 2
    io_wait();

    // ICW4
    outb(MPIC_DATA, ICW4_UPM);
    io_wait();
    outb(SPIC_DATA, ICW4_UPM);
    io_wait();

    // OCW1 - set masks
    outb(MPIC_DATA, backup_m);
    io_wait();
    outb(SPIC_DATA, backup_s);
    io_wait();

    // Disable timer...for now
    irq_set_mask(0);
    // Enable keyboard interrupts
    irq_clear_mask(1);
    asm("sti");

    // io_wait()'s ensure old machines have 
    // enough time for PIC to react to commands.
}

// OCW2 - signal EOI
void pic_send_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(SPIC_COMMAND, PIC_EOI);
    outb(MPIC_COMMAND, PIC_EOI);
}

void irq_set_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;
 
    if(irq < 8) {
        port = MPIC_DATA;
    } else {
        port = SPIC_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);        
}
 
void irq_clear_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;
 
    if(irq < 8) {
        port = MPIC_DATA;
    } else {
        port = SPIC_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);        
}

uint8_t pic_scan_code() {
    return inb(0x60);
}
