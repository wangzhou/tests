#!/bin/bash

#
# Just configure the Image/fs/qemu as fixed path
#
#
#-initrd ~/rootfs/rootfs.cpio.gz \
#-device virtio-net-device,netdev=net0 \
#-netdev type=user,id=net0,hostfwd=tcp::5555-:22 \


qemu-system-x86_64 -cpu host -enable-kvm -smp 4 \
-m 1G \
-kernel ~/repos/linux/arch/x86/boot/bzImage \
-append "console=ttyS0 root=/dev/sda1" \
-hda ./debian.img \
#-nographic \
#-drive if=none,file=./debian.img,format=qcow2,id=hd \
#-device virtio-blk-pci,drive=hd \

