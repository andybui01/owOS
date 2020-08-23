#!/bin/sh
echo -n "assemble boot.s ... " && i686-elf-as boot.s -o boot.o && echo "yes" || echo "no"
echo -n "compile kernel.c ... " && i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra && echo "yes" || echo "no"
echo -n "link objects ... " && i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc && echo "yes" || echo "no"

echo -n "multiboot spec ... "
if grub-file --is-x86-multiboot myos.bin; then
    echo yes
else
    echo no
    exit 1
fi

echo ready to connect on localhost:5900
qemu-system-i386 -vnc :0 -kernel myos.bin