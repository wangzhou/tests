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
-device ioh3420,id=root_port1 \
-object memory-backend-file,id=mb1,size=1M,share,mem-path=/dev/shm/ivshmem \
-device ivshmem-plain,memdev=mb1,bus=root_port1 \
-nographic \
