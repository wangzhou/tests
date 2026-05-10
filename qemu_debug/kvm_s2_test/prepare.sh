#!/bin/sh

echo 9000 > /sys/kernel/mm/hugepages/hugepages-64kB/nr_hugepages
mkdir /dev/hugepages
mount -t hugetlbfs hugetlbfs /dev/hugepages
mount -t debugfs none /sys/kernel/debug

echo 1 > /sys/kernel/debug/tracing/events/kvm/kvm_pgtable_visit/enable
echo 1 > /sys/kernel/debug/tracing/events/kvm/kvm_pgtable_walk/enable
echo 1 > /sys/kernel/debug/tracing/events/kvm/kvm_stage2_map/enable

ifconfig eth0 192.168.122.2 up

