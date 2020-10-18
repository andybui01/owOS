#!/bin/sh
set -e
. ./iso.sh

num=1

echo "starting owos on localhost:590$num"
# qemu-system-$(./target-triplet-to-arch.sh $HOST) -vnc :$num -cdrom owos.iso
qemu-system-$(./target-triplet-to-arch.sh $HOST) -no-reboot -no-shutdown -monitor stdio -vnc :$num -cdrom owos.iso
