#!/bin/bash
#
# 网络拓扑（Tap + Bridge）：
#
#     host: bridge br0 (192.168.100.1)
#              │              │
#              │ tap0         │ tap1
#              ▼              ▼
#         src 外层 VM    dst 外层 VM
#        (192.168.100.10)  (192.168.100.20)
#              │              │
#         ─ ─ ─│─ ─ ─ ─   ─ ─ ─│─ ─ ─ ─
#              ▼              ▼
#         内层 QEMU ←──热迁移──→ 内层 QEMU
#
# 前置条件: sudo ./setup_bridge.sh

qemu-system-aarch64 \
  -machine virt,gic-version=3,acpi=on,virtualization=true \
  -cpu max \
  -accel tcg \
  -m 4G \
  -smp 4 \
  -append "root=/dev/vda2 rw console=ttyAMA0" \
  -kernel /home/wz/linux/arch/arm64/boot/Image \
  -drive file=./openEuler-24.03-LTS-SP2-aarch64_dst.qcow2,format=qcow2,if=virtio \
  -nographic \
  -netdev tap,id=net0,ifname=tap1,script=no,downscript=no \
  -device virtio-net-pci,netdev=net0,mac=52:54:00:12:34:02 \
