#ifndef _INT_INT_H
#define _INT_INT_H

#define INT_DIVIDE_ZERO         0x00 // Divide by 0
#define INT_NMI                 0x02 // NMI Interrupt
#define INT_BREAKPOINT          0x03 // Breakpoint (INT3)
#define INT_OVERFLOW            0x04 // Overflow (INTO)
#define INT_BOUNDS_EXCEEDED     0x05 // Bounds range exceeded (BOUND)
#define INT_INVALID_OP          0x06 // Invalid opcode (UD2)
#define INT_DEV_NA              0x07 // Device not available (WAIT/FWAIT)
#define INT_DOUBLE_FAULT        0x08 // Double fault
#define INT_SEG_OVERRUN         0x09 // Coprocessor segment overrun
#define INT_BAD_TSS             0x0A // Invalid TSS
#define INT_NO_SEG              0x0B // Segment not present
#define INT_SS_FAULT            0x0C // Stack-segment fault
#define INT_GPF                 0x0D // General protection fault
#define INT_PAGE_FAULT          0x0E // Page fault
#define INT_FPU                 0x10 // x87 FPU error
#define INT_ALIGNMENT           0x11 // Alignment check
#define INT_MACHINE             0x12 // Machine check
#define INT_SIMD                0x13 // SIMD Floating-Point Exception

#define INT_KBD                 0x21 // Keyboard

#define INT_SYSCALL             0x80 // Syscall interrupt
#endif
