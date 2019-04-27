#!/bin/bash

#
# Just configure the Image/fs/qemu as fixed path
#
#
#-initrd ~/rootfs/rootfs.cpio.gz \
#-device virtio-net-device,netdev=net0 \
#-netdev type=user,id=net0,hostfwd=tcp::5555-:22 \


qemu-system-aarch64 -machine virt -cpu cortex-a57 \
-m 2G \
-kernel ~/repos/kernel-dev/arch/arm64/boot/Image \
-initrd ./rootfs.cpio.gz \
-append "console=ttyAMA0 root=/dev/ram rdinit=/init" \
-fsdev local,id=p9fs,path=/home/wangzhou/tests/kernel_debug_using_qemu/p9root,security_model=mapped \
-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9 \
-nographic \
-gdb tcp::1234 \
