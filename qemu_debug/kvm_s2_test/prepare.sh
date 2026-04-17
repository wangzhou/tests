#!/bin/sh

echo 9000 > /sys/kernel/mm/hugepages/hugepages-64kB/nr_hugepages
mkdir /dev/hugepages
mount -t hugetlbfs hugetlbfs /dev/hugepages
mount -t debugfs none /sys/kernel/debug

ifconfig eth0 192.168.122.2 up

