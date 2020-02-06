#!/bin/bash

#
# Just configure the Image/fs/qemu as fixed path
#
#
#-initrd ~/rootfs/rootfs.cpio.gz \
#-device virtio-net-device,netdev=net0 \
#-netdev type=user,id=net0,hostfwd=tcp::5555-:22 \


~/repos/qemu/aarch64-softmmu/qemu-system-aarch64 -machine virt -cpu cortex-a57 \
-m 2G \
-numa node,nodeid=0,mem=512,cpus=0-1 \
-numa node,nodeid=1,mem=512,cpus=2-3 \
-numa node,nodeid=2,mem=512,cpus=4-5 \
-numa node,nodeid=3,mem=512,cpus=6-7 \
-smp 8 \
-kernel ~/repos/linux/arch/arm64/boot/Image \
-initrd ~/repos/buildroot/output/images/rootfs.cpio.gz \
-append "console=ttyAMA0 root=/dev/ram rdinit=/init zswap.enable=1" \
-fsdev local,id=p9fs,path=/home/wangzhou/p9root,security_model=mapped \
-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9 \
-nographic \
# -gdb tcp::1234 \
# -device qm \
# -hdb ./disk.img \
#-initrd ~/repos/buildroot/output/images/rootfs.cpio.gz \
