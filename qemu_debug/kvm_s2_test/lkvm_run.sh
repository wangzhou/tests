#!/bin/sh

./lkvm run -k ./Image -d ./rootfs.ext4 -m 128 -c 2 --hugetlbfs /dev/hugepages -p "console=ttyS0 root=/dev/vda init=/bin/sh"

#./lkvm run -k ./Image --initrd ./rootfs.cpio.gz -m 128 -c 2 --hugetlbfs /dev/hugepages -p "console=ttyS0 root=/dev/ram0 rw init=/bin/sh"
