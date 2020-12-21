#include <mem/memlayout.h>

ENTRY (_start)

SECTIONS
{
    . = KMEM_LINK;
	_kernel_start = .;
	.text ALIGN(4K) : AT(V2P_WO(ADDR(.text)))
	{
		*(.multiboot)
		*(.text)
	}
	.rodata ALIGN(4K) : AT(V2P_WO(ADDR(.rodata)))
	{
		*(.rodata)
	}
	.data ALIGN(4K) : AT(V2P_WO(ADDR(.data)))
	{
		*(.data)
	}
	.bss ALIGN(4K) : AT(V2P_WO(ADDR(.bss)))
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
	_kernel_end = .;
}
