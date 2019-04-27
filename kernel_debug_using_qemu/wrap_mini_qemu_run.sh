#!/bin/bash

#
# Just configure the Image/fs/qemu as fixed path
#
#

~/repos/qemu/aarch64-softmmu/qemu-system-aarch64 -machine virt -cpu cortex-a57 \
-m 300M \
-kernel ~/repos/kernel-dev/arch/arm64/boot/Image \
-initrd ~/rootfs/rootfs.cpio.gz \
-append "console=ttyAMA0 root=/dev/ram rdinit=/init" \
-nographic \
