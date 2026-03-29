#!/bin/sh

#if use drive which if=ide, set root to /dev/sda1
#if use drive which if=virtio, set root to /dev/vda1

# P9PATH=p9root
# P9="-fsdev local,id=p9fs,path=$P9PATH,security_model=mapped \
# 	-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9"
# NET="-netdev type=tap,id=t1 -device virtio-net-pci,netdev=t1"

../build/qemu-system-riscv64 \
	-m 4096m \
	-nographic \
	-smp 8 \
	-machine virt \
	-kernel ~/repos/linux/arch/riscv/boot/Image \
	-append "console=ttyS0 root=/dev/ram rdinit=/init" \
	-initrd ~/repos/buildroot/output/images/rootfs.cpio.gz \
	-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9,bus=pcie.0 \
	-fsdev local,id=p9fs,path=/home/sherlock/p9root,security_model=mapped \
        -numa node,nodeid=0,mem=2048M,cpus=0-3 \
        -numa node,nodeid=1,mem=2048M,cpus=4-7
#	-S -gdb tcp::1234
#	-d exec,in_asm -D ~/qemu_log \
#	-bios none
#	-bios ~/repos/opensbi/build/platform/generic/firmware/fw_dynamic.elf
#	-plugin ../build/contrib/plugins/libcache.so \
#	-d plugin
#	-d exec,in_asm -D ~/qemu_log \
#	-bios none
#	-bios ~/repos/opensbi/build/platform/generic/firmware/fw_dynamic.elf
#	-S -gdb tcp::4321

#	-d exec,in_asm -D ~/qemu_log \
#	-machine virt -cpu 'rv64,x-h=true' \
#	-smp 4
#	-cpu sifive-u34 \
#	-dtb ./rv_dtb.dtb \
#	-machine virt,aclint=on -cpu 'rv64,h=true' \
