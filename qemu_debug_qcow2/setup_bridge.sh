#!/bin/bash
#
# host: bridge br0 (192.168.100.1)
#          │              │
#          │ tap0         │ tap1
#          ▼              ▼
#     src 外层 VM    dst 外层 VM
#    (192.168.100.10)  (192.168.100.20)
#          │              │
#     ─ ─ ─│─ ─ ─ ─   ─ ─ ─│─ ─ ─ ─
#          ▼              ▼
#     内层 QEMU ←──热迁移──→ 内层 QEMU
#
# 本脚本需要 root 权限执行，用于创建 bridge 和 tap 设备

set -e

BRIDGE=br0
BRIDGE_IP=192.168.100.1
TAP0=tap0
TAP1=tap1

# 清理已存在的设备（忽略错误）
ip link del "$TAP0" 2>/dev/null || true
ip link del "$TAP1" 2>/dev/null || true
ip link del "$BRIDGE" 2>/dev/null || true

# 创建 bridge
ip link add "$BRIDGE" type bridge
ip addr add "${BRIDGE_IP}/24" dev "$BRIDGE"
ip link set "$BRIDGE" up

# 创建 tap 设备并接入 bridge
ip tuntap add "$TAP0" mode tap
ip tuntap add "$TAP1" mode tap

ip link set "$TAP0" master "$BRIDGE"
ip link set "$TAP1" master "$BRIDGE"

ip link set "$TAP0" up
ip link set "$TAP1" up

echo "Bridge and tap devices created:"
ip link show "$BRIDGE"
ip link show "$TAP0"
ip link show "$TAP1"
echo ""
echo "Bridge IP: ${BRIDGE_IP}/24"
echo ""
echo "Guest network config (configure inside each VM):"
echo "  src VM: ip addr add 192.168.100.10/24 dev eth0 && ip link set eth0 up"
echo "  dst VM: ip addr add 192.168.100.20/24 dev eth0 && ip link set eth0 up"
echo ""
echo "Default gateway (optional): ip route add default via ${BRIDGE_IP}"
