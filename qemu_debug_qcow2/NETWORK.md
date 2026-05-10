# QEMU 双虚机桥接网络环境

## 拓扑

```
host: bridge br0 (192.168.100.1)
         │              │
         │ tap0         │ tap1
         ▼              ▼
    src 外层 VM    dst 外层 VM
   (192.168.100.10)  (192.168.100.20)
   MAC: ...:34:01    MAC: ...:34:02
         │              │
    ─ ─ ─│─ ─ ─ ─   ─ ─ ─│─ ─ ─ ─
         ▼              ▼
    内层 QEMU ←──热迁移──→ 内层 QEMU
```

## 文件

| 文件 | 用途 |
|------|------|
| `setup_bridge.sh` | 创建 br0 + tap0/tap1，需要 sudo |
| `run_qemu_qcow2_src.sh` | 启动 src 虚机，tap0 接入 |
| `run_qemu_qcow2_dst.sh` | 启动 dst 虚机，tap1 接入 |

## 网络参数

| 设备 | IP | MAC |
|------|-----|-----|
| host br0 | 192.168.100.1/24 | - |
| src VM | 192.168.100.10/24 | 52:54:00:12:34:01 |
| dst VM | 192.168.100.20/24 | 52:54:00:12:34:02 |

## 使用步骤

```bash
# 1. 一次性创建 bridge（需要 root）
sudo ./setup_bridge.sh

# 2. 分别启动虚机（两个终端）
./run_qemu_qcow2_src.sh    # 终端1
./run_qemu_qcow2_dst.sh    # 终端2

# 3. 虚机内配 IP
# src 虚机:
ip addr add 192.168.100.10/24 dev eth0 && ip link set eth0 up
# dst 虚机:
ip addr add 192.168.100.20/24 dev eth0 && ip link set eth0 up
```

## 当前能力

- host ↔ src ✓
- host ↔ dst ✓
- src ↔ dst ✓
- 虚机 → 外网 ✗（未配 NAT/IP 转发）

## 注意事项

- QEMU `-device` 必须显式指定不同的 `mac=`，否则默认 MAC 冲突会导致虚机之间不通
- 如果虚机内 `eth0` 不存在，用 `ip link show` 确认实际接口名（如 `enp0s1`）
