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
	-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9,bus=pcie.0 \
	-fsdev local,id=p9fs,path=./kvm_s2_test,security_model=mapped \
	-device virtio-net-pci,netdev=mynet1,id=net0 \
	-netdev tap,id=mynet1,script=./qemu-ifup-nat,downscript=./qemu-ifdown-nat

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
