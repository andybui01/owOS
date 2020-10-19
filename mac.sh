#!/bin/sh
set -e
. ./iso.sh

objcopy --only-keep-debug kernel/owos.kernel kernel.sym

# qemu-system-i386 -no-reboot -s -S -kernel sysroot/boot/owos.kernel
qemu-system-i386 -no-reboot -no-shutdown -monitor stdio -kernel sysroot/boot/owos.kernel