#!/bin/bash

#
# $1 file path
# $2 directory to put in qemu fs
#

gunzip ~/rootfs/rootfs.cpio.gz
mv ~/rootfs/rootfs.cpio /tmp
mkdir -p /tmp/rootfs
cd /tmp/rootfs
cpio -ivmd < ../rootfs.cpio
rm ../rootfs.cpio

cp $1 /tmp/rootfs$2

find . | cpio -o --format=newc > ../rootfs.cpio
cd ..
rm -r /tmp/rootfs
gzip -c ./rootfs.cpio > ~/rootfs/rootfs.cpio.gz
rm ./rootfs.cpio
