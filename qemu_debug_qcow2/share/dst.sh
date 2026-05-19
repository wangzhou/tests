#!/bin/sh

./qemu-system-aarch64 \
	-enable-kvm \
	-m 256 \
	-smp 4 \
	-cpu host \
	-machine virt,gic-version=3 \
	-L /usr/share/qemu \
	-append "console=ttyAMA0 root=/dev/ram init=/init" \
	-nographic \
	-kernel ./Image \
	-initrd ./rootfs_mini.cpio.gz \
	-mem-patch /dev/hugepages \
	-incoming tcp:192.168.100.20:6666
