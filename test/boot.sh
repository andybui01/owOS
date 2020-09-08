rm *.o && echo "remove .o"
echo -n "assemble boot.s ... " && i686-elf-as boot.S -o boot.o && echo "yes" || echo "no"
echo -n "assemble gdt_flush.s ... " && i686-elf-as gdt_flush.S -o gdt_flush.o && echo "yes" || echo "no"
echo -n "compile kernel.c ... " && i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra && echo "yes" || echo "no"
echo -n "compile gdt.c ... " && i686-elf-gcc -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra && echo "yes" || echo "no"
echo -n "compile tty.c ... " && i686-elf-gcc -c tty.c -o tty.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra && echo "yes" || echo "no"
echo -n "link objects ... " && i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o gdt.o tty.o gdt_flush.o -lgcc && echo "yes" || echo "no"

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