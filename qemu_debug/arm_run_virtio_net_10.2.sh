#!/bin/sh

../build/qemu-system-aarch64 \
	-smp cpus=2,maxcpus=4 \
	-cpu max \
	-machine virt,gic-version=3 \
 	-append "console=ttyAMA0 nokaslr" \
	-nographic -m 4096m \
	-kernel /home/sherlock/repos/linux/arch/arm64/boot/Image \
	-initrd /home/sherlock/repos/buildroot/output/images/rootfs.cpio.gz \
	-netdev tap,id=net,script=./qemu-ifup-nat-10.2,downscript=./qemu-ifdown-nat \
	-device virtio-net-pci,netdev=net,id=net1
	#-initrd /home/sherlock/rootfs.cpio.gz \
