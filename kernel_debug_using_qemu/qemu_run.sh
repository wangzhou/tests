#!/bin/bash

#
# Just configure the Image/fs/qemu as fixed path
#
#

~/repos/qemu/aarch64-softmmu/qemu-system-aarch64 -machine virt -cpu cortex-a57 \
-kernel ~/repos/linux/arch/arm64/boot/Image \
-drive if=none,file=/home/sherlock/Downloads/openembedded_sys/fs.img,id=fs \
-device virtio-blk-device,drive=fs \
-append 'console=ttyAMA0 root=/dev/vda2' \
-nographic \
#-net nic -net tap,ifname=tap0,script=no,downscript=no
