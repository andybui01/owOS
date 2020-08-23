#!/bin/sh
set -e
. ./iso.sh

echo "starting owos on localhost:5900"
qemu-system-$(./target-triplet-to-arch.sh $HOST) -vnc :0 -cdrom owos.iso
