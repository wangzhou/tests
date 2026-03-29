#!/bin/sh

#if use drive which if=ide, set root to /dev/sda1
#if use drive which if=virtio, set root to /dev/vda1

#P9PATH=~/work/hisi-repo/warpdrive/test
P9PATH=p9root

#~/work/qemu-run-arm64/qemu/aarch64-softmmu/qemu-system-aarch64 \
#	-s cpu cortex-a57 -machine virt \
#	-nographic -smp 2 -m 1024m -kernel arch/arm64/boot/Image \
#	-netdev type=user,id=net0,hostfwd=tcp::5555-:22 \
#	-netdev type=user,id=net1 \
#	-netdev type=user,id=net2 \
#	-device pcie-root-port,id=pcie.1,bus=pcie.0,port=1,chassis=1,slot=0 \
#	-device pcie-root-port,id=pcie.2,bus=pcie.0,port=2,chassis=2,slot=0 \
#	-device virtio-net-pci,bus=pcie.0,netdev=net0,addr=5.0 \
#	-device virtio-net-pci,bus=pcie.1,netdev=net1,addr=0.0 \
#	-device virtio-net-pci,bus=pcie.2,netdev=net2,addr=0.0 \
#	-device virtio-net-pci,? \
#	-append "console=ttyAMA0"

../build/qemu-system-aarch64 \
	-smp 4 \
	-cpu max \
	-machine virt \
 	-append "console=ttyAMA0 nokaslr" \
	-nographic -m 4096m \
	-kernel ~/repos/linux/arch/arm64/boot/Image \
	-initrd /home/sherlock/repos/buildroot/output/images/rootfs.cpio.gz \
	-incoming tcp:0:6666 \
	#-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9,bus=pcie.0 \
	#-fsdev local,id=p9fs,path=/home/sherlock/p9root,security_model=mapped \
        #-serial telnet:localhost:9000,server,nowait \
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

#These are tested:
# with external image
#qemu-system-aarch64 -cpu cortex-a57 -machine virt \
#	-S -s \
#	-drive if=none,file=ubuntu-14.04-server-cloudimg-arm64-uefi1.img,id=hd0 \
#	-device virtio-blk-device,drive=hd0 \
#	-nographic -smp 1 -m 1024m -kernel arch/arm64/boot/Image \
#	-append "console=ttyAMA0 root=/dev/vda1 init=/bin/sh"
#
# with buildroot as initramfs
#qemu-system-aarch64 -cpu cortex-a57 -machine virt \
#	-nographic -smp 1 -m 1024m -kernel arch/arm64/boot/Image \
#	-device virtio-net-pci,netdev=net0 -netdev type=user,id=net0,hostfwd=tcp::5555-:22 \
#	-fsdev local,id=p9fs,path=p9root,security_model=mapped \
#	-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9 \
#	-append "console=ttyAMA0"
#
# with a user net:
#	the dhcp address of the guest is 10.0.2.15,
#	proxy is 100.0.2.2
#	dhcp server is 10.0.2.3
# with hostfwd:
#	ssh to local port will be redirect to guest port
# with plan 9 filesystem, mount in guest by:
#	mount -t 9p -o trans=virtio p9 /mnt
# to enable pr_debug (after enable dyndbg in kernel) add following commands:
#	dyndbg="file xxxx.c +p; file xxx.c +p"
# this enable pr_dubug only, other pr_xxx is not controlled by it.

# ../qemu-mainline/build/qemu-system-aarch64 \
# 	-s -cpu cortex-a57 -machine virt,gic-version=3,iommu=smmuv3 \
# 	-nographic -smp 2 -m 1024m \
# 	-kernel arch/arm64/boot/Image \
# 	-device virtio-net-pci,netdev=net0 \
# 	-netdev type=user,id=net0,hostfwd=tcp::5555-:22 \
# 	-d in_asm -D log \
# 	-device bbox-proxy -device ghms_pci \
# 	-append "console=ttyAMA0 nokaslr loglevel=7"
