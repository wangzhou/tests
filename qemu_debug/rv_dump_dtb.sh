#!/bin/sh

#if use drive which if=ide, set root to /dev/sda1
#if use drive which if=virtio, set root to /dev/vda1

# P9PATH=p9root
# P9="-fsdev local,id=p9fs,path=$P9PATH,security_model=mapped \
# 	-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9"
# NET="-netdev type=tap,id=t1 -device virtio-net-pci,netdev=t1"

#../build/qemu-system-riscv64 -machine virt,aclint=on,aia=aplic -machine dumpdtb=rv_dtb.dtb \
../build/qemu-system-riscv64 -machine virt,aclint=on,aia=aplic \
	-dtb ./rv_dtb.dtb \
	-m 256M \
	-smp 8 \
	-kernel ~/repos/linux/arch/riscv/boot/Image \
	-initrd ~/repos/buildroot/output/images/rootfs.cpio.gz
#        -numa node,nodeid=0,mem=128M,cpus=0-3 \
#        -numa node,nodeid=1,mem=128M,cpus=4-7 \
# dtc rv_dtb.dtb -I dtb -o rv.dts

#aia=imsic
#../build/qemu-system-riscv64 -machine virt,aclint=on,aia=aplic-imsic -machine dumpdtb=rv_dtb.dtb \
