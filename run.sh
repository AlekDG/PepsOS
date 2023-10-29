#!/bin/bash
if [[ "$1" = "gdb" ]]; then
sudo qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -soundhw pcspk -d int
else
sudo qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -soundhw pcspk
fi