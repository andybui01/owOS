#!/bin/sh
set -e
. ./iso.sh

num=1

objcopy --only-keep-debug kernel/owos.kernel kernel.sym

echo "starting owos on localhost:590$num"
echo "connect with gdb to localhost:1234"
qemu-system-$(./target-triplet-to-arch.sh $HOST) -monitor stdio -vnc :$num -s -S -cdrom owos.iso
