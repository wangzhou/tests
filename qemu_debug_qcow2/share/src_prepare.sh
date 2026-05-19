#!/bin/sh

echo 18000 > /sys/kernel/mm/hugepages/hugepages-64kB/nr_hugepages
mount -t hugetlbfs hugetlbfs /dev/hugepages
mount -t debugfs none /sys/kernel/debug

echo 1 > /sys/kernel/debug/tracing/events/kvm/kvm_pgtable_visit/enable
echo 1 > /sys/kernel/debug/tracing/events/kvm/kvm_pgtable_walk/enable
echo 1 > /sys/kernel/debug/tracing/events/kvm/kvm_stage2_map/enable

systemctl stop NetworkManager
systemctl disable NetworkManager

ip addr add 192.168.100.10/24 dev enp0s1 && ip link set enp0s1 up
