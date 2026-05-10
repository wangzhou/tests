#!/bin/sh

/home/wz/qemu/build/qemu-system-aarch64 \
	-m 4G \
	-smp 4 \
	-cpu max \
	-machine virt,gic-version=3,acpi=on,virtualization=true \
	-append "console=ttyAMA0 root=/dev/ram init=/init default_hugepagesz=64K" \
	-nographic \
	-kernel /home/wz/linux/arch/arm64/boot/Image \
	-initrd /home/wz/tests/qemu_debug/rootfs_sshd_new.cpio.gz \
	-device hisi-zip,bus=pcie.0 \
	-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9,bus=pcie.0 \
	-fsdev local,id=p9fs,path=./kvm_s2_test,security_model=mapped
