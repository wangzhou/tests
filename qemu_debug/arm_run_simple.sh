#!/bin/sh

qemu-system-aarch64 \
	-smp 1 \
	-cpu max  \
	-machine virt \
 	-append "console=ttyAMA0 nokaslr root=/dev/vda1 init=/sbin/init" \
	-nographic \
	-m 4096m,slots=5,maxmem=20G \
	-kernel /home/sherlock/repos/linux/arch/arm64/boot/Image \
	-initrd /home/sherlock/repos/buildroot/output/images/rootfs.cpio.gz \
#	-chardev stdio,mux=on,logfile=./qemu_log,id=char0 \
#	-serial chardev:char0
	#-display none \
#	-daemonize
#	-mon chardev=char0 
#	-bios ~/QEMU_EFI.fd \
#	-bios ../pc-bios/edk2-aarch64-code.fd \
#	-initrd /home/sherlock/rootfs.cpio.gz \
#	-s -S
	#-cpu cortex-a57  \

	#-qmp unix:/opt/qmp.socket,server,nowait
	#-initrd /home/sherlock/rootfs.cpio.gz \
#	-smp 8,maxcpus=8,sockets=1,clusters=2,cores=2,threads=2 \
