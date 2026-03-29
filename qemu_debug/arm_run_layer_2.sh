#!/bin/bash

../build/qemu-system-aarch64 \
	-smp 1 \
	-cpu host \
	-accel kvm \
	-machine virt \
 	-append "console=ttyAMA0 nokaslr" \
	-nographic -m 2G \
	-kernel /home/sherlock/repos/linux/arch/arm64/boot/Image \
	-initrd /home/sherlock/rootfs.cpio.gz \

	#-initrd /home/sherlock/rootfs.cpio.gz \
	#-bios ../pc-bios/edk2-aarch64-code.fd \
