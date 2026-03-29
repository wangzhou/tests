#!/bin/sh

#if use drive which if=ide, set root to /dev/sda1
#if use drive which if=virtio, set root to /dev/vda1

P9PATH=p9root

../build/qemu-system-aarch64 \
	-smp 1 \
	-cpu max \
	-machine virt,virtualization=on,nvdimm=on \
 	-append "console=ttyAMA0 nokaslr" \
	-nographic -m 4096m \
	-kernel /home/sherlock/repos/linux/arch/arm64/boot/Image \
	-initrd /home/sherlock/repos/buildroot/output/images/rootfs.cpio.gz \
	-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9,bus=pcie.0 \
	-fsdev local,id=p9fs,path=/home/sherlock/p9root,security_model=passthrough \
	#-s -S
	#-D ./log -d in_asm,cpu
	#-fsdev local,id=p9fs,path=/home/sherlock/p9root,security_model=mapped \
	#-netdev tap,id=mynet,script=./qemu-ifup-nat,downscript=./qemu-ifdown-nat \
	#-device virtio-net-pci,netdev=mynet,id=net1
	#-s -S
	#-object memory-backend-ram,id=mem0,size=2048M \
	#-object memory-backend-ram,id=mem1,size=2048M \
	#-hda block.img \
        #-gdb tcp::1234
	#-plugin ~/repos/qemu/build/contrib/plugins/libexeclog.so \
	#-d plugin -D ~/log
	#-smp cores=8,threads=1,sockets=2 \
	#-smp cpus=16,maxcpus=16,sockets=2,clusters=4,threads=2 \
	#-append "console=ttyAMA0,earlycon=pl011,0x20201000" \
	#-append "console=ttyAMA0" \
	#-initrd ~/tests/kernel_debug_using_qemu/rootfs.cpio.gz \

	#-smp 16,sockets=2,clusters=2,threads=2 \
        #-numa node,memdev=mem0,nodeid=0,cpus=0-7 \
        #-numa node,memdev=mem1,nodeid=1,cpus=8-15 \

