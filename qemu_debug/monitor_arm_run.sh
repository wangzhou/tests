#!/bin/sh

../build/qemu-system-aarch64 \
	-smp 2\
	-cpu max  \
	-machine virt \
 	-append "console=ttyAMA0 nokaslr" \
	-nographic -m 4096m \
	-kernel /home/sherlock/repos/linux/arch/arm64/boot/Image \
	-initrd /home/sherlock/repos/buildroot/output/images/rootfs.cpio.gz \
	-monitor unix:/tmp/qemu_moniter,server,nowait

