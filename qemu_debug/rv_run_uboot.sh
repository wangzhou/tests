#!/bin/sh

#if use drive which if=ide, set root to /dev/sda1
#if use drive which if=virtio, set root to /dev/vda1

# P9PATH=p9root
# P9="-fsdev local,id=p9fs,path=$P9PATH,security_model=mapped \
# 	-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9"
# NET="-netdev type=tap,id=t1 -device virtio-net-pci,netdev=t1"

../build/qemu-system-riscv64 \
	-m 256m \
	-machine virt \
	-nographic \
	-kernel /home/sherlock/repos/u-boot/u-boot \
        -drive file=/home/sherlock/rootfs.img,format=raw,id=hd0 \
	-device virtio-blk-device,drive=hd0 \
	-append "root=/dev/vda rw console=ttyS0"


#-kernel /home/sherlock/repos/linux/arch/riscv/boot/Image \
