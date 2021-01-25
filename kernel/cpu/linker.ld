
ENTRY (_start)

SECTIONS
{
    . = (0xC0000000 + 0x00100000);
 _kernel_start = .;
 .text ALIGN(4K) : AT(((ADDR(.text)) - 0xC0000000))
 {
  *(.multiboot)
  *(.text)
 }
 .rodata ALIGN(4K) : AT(((ADDR(.rodata)) - 0xC0000000))
 {
  *(.rodata)
 }
 .data ALIGN(4K) : AT(((ADDR(.data)) - 0xC0000000))
 {
  *(.data)
 }
 .bss ALIGN(4K) : AT(((ADDR(.bss)) - 0xC0000000))
 {
  *(COMMON)
  *(.bss)
  *(.bootstrap_stack)
 }
 _kernel_end = .;
}
