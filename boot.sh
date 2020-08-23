#!/bin/sh
echo -n "assemble boot.s ... "
if i686-elf-as boot.s -o boot.o; then
    echo "yes"
else
    echo "no"
    exit 1
fi

echo -n "compile kernel.c ... "
if i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra; then
    echo "yes"
else
    echo "no"
    exit 1
fi

echo -n "link objects ... "
if i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc;then
    echo "yes"
else
    echo "no"
    exit 1
fi

echo -n "multiboot spec ... "
if grub-file --is-x86-multiboot myos.bin; then
    echo yes
else
    echo no
    exit 1
fi

mkdir -p isodir/boot/grub
cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir

echo ready to connect on localhost:5900
qemu-system-i386 -vnc :0 -cdrom myos.iso