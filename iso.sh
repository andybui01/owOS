#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/owos.kernel isodir/boot/owos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
GRUB_GFXMODE=1024x768
menuentry "owos" {
	multiboot /boot/owos.kernel
}
EOF
grub-mkrescue -o owos.iso isodir
